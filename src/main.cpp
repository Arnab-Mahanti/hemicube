#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string_view>
#include <fstream>
#include <sstream>

#define GL_ASSERT(x) \
    if (!(x))        \
        __debugbreak();
#define GL_Call(x)  \
    GLClearError(); \
    x;              \
    GL_ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR)
    {
        /* code */
    }
}

static bool GLLogCall(const char *function, const char *file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error]: (" << error << ") "
                  << function << " " << file << " " << line
                  << std::endl;
        return false;
    }
    return true;
}

GLuint createShader(const std::string &vertexShader, const std::string &fragmentShader)
{

    auto program = glCreateProgram();

    auto vs = glCreateShader(GL_VERTEX_SHADER);
    auto fs = glCreateShader(GL_FRAGMENT_SHADER);

    auto vsrc = vertexShader.c_str();
    auto fsrc = fragmentShader.c_str();

    int result;

    glShaderSource(vs, 1, &vsrc, nullptr);
    glShaderSource(fs, 1, &fsrc, nullptr);

    glCompileShader(vs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &length);
        char *message = (char *)alloca(length * sizeof(char));
        glGetShaderInfoLog(vs, length, &length, message);
        std::cout << "[ERROR]: Vertex Shader: " << message << "\n";
        glDeleteShader(vs);
        return 0;
    }

    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &length);
        char *message = (char *)alloca(length * sizeof(char));
        glGetShaderInfoLog(fs, length, &length, message);
        std::cout << "[ERROR]: Fragment Shader: " << message << "\n";
        glDeleteShader(fs);
        return 0;
    }

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(int, char **)
{
    if (!glfwInit())
    {
        glfwTerminate();
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    auto window = glfwCreateWindow(800, 600, "hemicube", NULL, NULL);
    if (!window)
    {
        const char *description;
        auto error = glfwGetError(&description);
        std::cout << "[ERROR]: " << error << " : " << description << "\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSwapInterval(2);
    std::cout << glGetString(GL_VERSION) << "\n";
    std::cout << glGetString(GL_RENDERER) << "\n";

    float positions[] = {
        -0.5f, -0.5f,
        -0.5f, 0.5f,
        0.5f, 0.5f,
        0.5f, -0.5f};

    uint32_t indices[] = {
        0, 1, 2,
        2, 3, 0};

    GLuint vao = 0;
    GL_Call(glGenVertexArrays(1, &vao));
    GL_Call(glBindVertexArray(vao));

    GLuint buffer = 0;
    GL_Call(glGenBuffers(1, &buffer));
    GL_Call(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GL_Call(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW));
    GL_Call(glEnableVertexAttribArray(0));
    GL_Call(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));

    GLuint ibo = 0;
    GL_Call(glGenBuffers(1, &ibo));
    GL_Call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GL_Call(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

    std::ifstream vertexShaderFile("C:/Users/Arnab Mahanti/source/repos/hemicube/src/shader.vert");
    std::ostringstream vertexStream;
    vertexStream << vertexShaderFile.rdbuf();
    std::ifstream fragmentShaderFile("C:/Users/Arnab Mahanti/source/repos/hemicube/src/shader.frag");
    std::ostringstream fragmentStream;
    fragmentStream << fragmentShaderFile.rdbuf();

    auto vertexShader = vertexStream.str();
    auto fragmentShader = fragmentStream.str();

    std::cout << vertexShader << "\n";
    std::cout << fragmentShader << "\n";

    auto shader = createShader(vertexShader, fragmentShader);

    // glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    float r = 0.0f;
    float increment = 0.05f;
    while (!glfwWindowShouldClose(window))
    {
        GL_Call(glClear(GL_COLOR_BUFFER_BIT));

        GL_Call(glBindVertexArray(vao));
        GL_Call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

        GL_Call(glUseProgram(shader));

        GL_Call(glUniform4f(glGetUniformLocation(shader, "u_Color"), r, 0.0f, 0.0f, 1.0f));
        GL_Call(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        if (r >= 1.0f)
            increment = -0.05f;
        if (r <= 0.0f)
            increment = 0.05f;

        r += increment;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    GL_Call(glDeleteProgram(shader));
    glfwDestroyWindow(window);
    glfwTerminate();
}
