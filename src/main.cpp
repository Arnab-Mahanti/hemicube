#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string_view>
#include <fstream>
#include <sstream>
#include <chrono>

#include "Utility.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

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
    glfwSwapInterval(4);

    glfwSetWindowSizeCallback(window, [](GLFWwindow *window, int width, int height)
                              { GL_Call(glViewport(0, 0, width, height)); });

    std::cout << glGetString(GL_VERSION) << "\n";
    std::cout << glGetString(GL_RENDERER) << "\n";

    {
        float positions[] = {
            -0.5f, -0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.0f, 1.0f,
            0.5f, 0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, 1.0f, 0.0f};

        uint32_t indices[] = {
            0, 1, 2,
            2, 3, 0};

        GL_Call(glEnable(GL_BLEND));
        GL_Call(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC1_ALPHA));

        VertexArray va;
        VertexBufferLayout layout;
        VertexBuffer vb(positions, sizeof(positions));
        layout.Push<float>(2);
        layout.Push<float>(2);

        va.AddBuffer(vb, layout);
        IndexBuffer ib(indices, 6);

        glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);

        Shader shader("C:/Users/Arnab Mahanti/source/repos/hemicube/res/shader/shader.vert",
                      "C:/Users/Arnab Mahanti/source/repos/hemicube/res/shader/shader.frag");
        Texture texture("C:/Users/Arnab Mahanti/source/repos/hemicube/res/texture/ISRO-logo.png");

        texture.Bind();
        shader.Bind();
        shader.SetUniform1i("u_Texture", 0);
        shader.SetUniformMat4f("u_MVP", proj);
        Renderer renderer;

        std::string title;
        while (!glfwWindowShouldClose(window))
        {
            auto start = std::chrono::high_resolution_clock::now();

            renderer.Clear();
            renderer.Draw(va, ib, shader);
            glfwSwapBuffers(window);

            auto frameTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count();
            title = "Hemicube;  FT: " + std::to_string(frameTime) + "ms; FPS: " + std::to_string(1000.0 / frameTime);
            glfwSetWindowTitle(window, title.c_str());

            glfwPollEvents();
        }
    }
    glfwDestroyWindow(window);

    glfwTerminate();
}
