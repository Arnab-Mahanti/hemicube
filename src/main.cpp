#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string_view>
#include <fstream>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

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

    {
        float positions[] = {
            -0.5f, -0.5f,
            -0.5f, 0.5f,
            0.5f, 0.5f,
            0.5f, -0.5f};

        uint32_t indices[] = {
            0, 1, 2,
            2, 3, 0};

        VertexArray va;
        VertexBufferLayout layout;
        VertexBuffer vb(positions, sizeof(positions));
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);
        IndexBuffer ib(indices, 6);
        Shader program("C:/Users/Arnab Mahanti/source/repos/hemicube/res/shader/shader.vert",
                       "C:/Users/Arnab Mahanti/source/repos/hemicube/res/shader/shader.frag");

        vb.Unbind();
        va.Unbind();
        ib.Unbind();
        program.Unbind();
        // glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

        float r = 0.0f;
        float increment = 0.05f;
        while (!glfwWindowShouldClose(window))
        {
            GL_Call(glClear(GL_COLOR_BUFFER_BIT));

            va.Bind();
            ib.Bind();
            program.Bind();
            program.SetUniform4f("u_Color", r, 0.0f, 0.0f, 1.0f);

            GL_Call(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            if (r >= 1.0f)
                increment = -0.05f;
            if (r <= 0.0f)
                increment = 0.05f;

            r += increment;

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    glfwDestroyWindow(window);

    glfwTerminate();
}
