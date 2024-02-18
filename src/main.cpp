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

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

static constexpr auto windowWidth = 800;
static constexpr auto windowHeight = 600;

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
    auto window = glfwCreateWindow(windowWidth, windowHeight, "hemicube", NULL, NULL);
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
    glfwSwapInterval(1);
    glfwSwapInterval(1);

    glfwSetWindowSizeCallback(window, [](GLFWwindow *window, int width, int height)
                              { GL_Call(glViewport(0, 0, width, height)); });

    std::cout << glGetString(GL_VENDOR) << "\n";
    std::cout << glGetString(GL_VENDOR) << "\n";
    std::cout << glGetString(GL_RENDERER) << "\n";
    std::cout << glGetString(GL_VERSION) << "\n";
    std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
    ImGui::StyleColorsDark();

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
        GL_Call(glEnable(GL_DEPTH_TEST));
        GL_Call(glDepthFunc(GL_LESS));
        GL_Call(glEnable(GL_DEPTH_TEST));
        GL_Call(glDepthFunc(GL_LESS));

        VertexArray va;
        VertexBufferLayout layout;
        VertexBuffer vb(positions, sizeof(positions));
        layout.Push<float>(2);
        layout.Push<float>(2);

        va.AddBuffer(vb, layout);
        IndexBuffer ib(indices, 6);

        Shader shader("C:/Users/Arnab Mahanti/source/repos/hemicube/res/shader/shader.vert",
                      "C:/Users/Arnab Mahanti/source/repos/hemicube/res/shader/shader.frag");
        Texture texture("C:/Users/Arnab Mahanti/source/repos/hemicube/res/texture/ISRO-logo.png");

        texture.Bind();
        shader.Bind();
        shader.SetUniform1i("u_Texture", 0);
        Renderer renderer;

        std::string title;
        int width, height;
        glm::vec3 redpos(0.f, 0.f, -10.f);
        constexpr auto speed = 0.05f;
        glm::vec3 redpos(0.f, 0.f, -10.f);
        constexpr auto speed = 0.05f;
        while (!glfwWindowShouldClose(window))
        {
            glfwGetWindowSize(window, &width, &height);
            auto start = std::chrono::high_resolution_clock::now();

            renderer.Clear();
            // auto proj = glm::scale(glm::ortho(-1.0f, 1.0f, -float(height) / width, float(height) / width, -1.0f, 1.0f), glm::vec3(0.5f, 0.5f, 1.0f));
            auto proj = glm::perspective(glm::radians(45.f), float(width) / height, .1f, 100.f);

            glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f));
            shader.SetUniformMat4f("u_MVP", proj * view);
            shader.SetUniform4f("u_Color", 0.0f, 1.f, 0.f, 1.f);
            renderer.Draw(va, ib, shader);

            view = glm::translate(glm::mat4(1.f), redpos);
            shader.SetUniformMat4f("u_MVP", proj * view);
            shader.SetUniform4f("u_Color", 1.0f, 0.f, 0.f, 1.f);
            shader.SetUniform4f("u_Color", 1.0f, 0.f, 0.f, 1.f);
            renderer.Draw(va, ib, shader);

            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                redpos.y += speed;
            else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                redpos.x -= speed;
            else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                redpos.y -= speed;
            else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                redpos.x += speed;
            else if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
                redpos.z += speed;
            else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
                redpos.z -= speed;

            ImGui_ImplGlfw_NewFrame();
            ImGui_ImplOpenGL3_NewFrame();
            ImGui::NewFrame();
            // ImGui::ShowDemoWindow();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);

            auto frameTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count();
            title = "Hemicube;  FT: " + std::to_string(frameTime) + "ms; FPS: " + std::to_string(1000.0 / frameTime);
            glfwSetWindowTitle(window, title.c_str());

            glfwPollEvents();
        }
    }
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);

    glfwTerminate();
}
