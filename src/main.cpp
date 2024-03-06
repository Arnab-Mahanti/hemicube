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

#include "Mesh.h"

static constexpr auto windowWidth = 800;
static constexpr auto windowHeight = 600;

int main(int, char **)
{
    if (!glfwInit())
    {
        glfwTerminate();
        return -1;
    }

    Mesh2D mesh("C:/Users/Arnab Mahanti/Downloads/hemicube/res/mesh/square_mesh.dat");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

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
    glfwSwapInterval(2);

    glfwSetWindowSizeCallback(window, [](GLFWwindow *window, int width, int height)
                              { GL_Call(glViewport(0, 0, width, height)); });

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
        // float positions[] = {
        //     -0.5f, -0.5f, 0.0f, 0.0f,
        //     -0.5f, 0.5f, 0.0f, 1.0f,
        //     0.5f, 0.5f, 1.0f, 1.0f,
        //     0.5f, -0.5f, 1.0f, 0.0f};

        // uint32_t indices[] = {
        //     0, 1, 2,
        //     2, 3, 0};

        GL_Call(glEnable(GL_BLEND));
        GL_Call(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GL_Call(glEnable(GL_DEPTH_TEST));
        GL_Call(glDepthFunc(GL_LESS));
        

        // VertexArray va;
        // VertexBufferLayout layout;
        // VertexBuffer vb(positions, sizeof(positions));
        // layout.Push<float>(2);
        // layout.Push<float>(2);

        // va.AddBuffer(vb, layout);
        // IndexBuffer ib(indices, 6);

        VertexArray va;
        VertexBufferLayout layout;
        VertexBuffer vb(mesh.GetVertices(), mesh.GetVertexSize());
        layout.Push<float>(3);
        layout.Push<float>(4);

        va.AddBuffer(vb, layout);
        IndexBuffer ib(mesh.GetIndices(), mesh.GetIndexCount());

        Shader shader("C:/Users/Arnab Mahanti/Downloads/hemicube/res/shader/shader.vert",
                      "C:/Users/Arnab Mahanti/Downloads/hemicube/res/shader/shader.frag");
        Texture texture("C:/Users/Arnab Mahanti/Downloads/hemicube/res/mesh/square_mesh.dat");

        texture.Bind();
        shader.Bind();
        // shader.SetUniform1i("u_Texture", 0);
        Renderer renderer;

        std::string title;
        int width, height;
        glm::vec3 redpos(0.f, 0.f, -100.f);
        constexpr auto relspeed = 0.03f;
        auto scale = 15000.f;
        while (!glfwWindowShouldClose(window))
        {
            glfwGetWindowSize(window, &width, &height);
            auto start = std::chrono::high_resolution_clock::now();

            renderer.Clear();

            auto proj = glm::perspective(glm::radians(45.f), float(width) / height, .1f, scale);
            // auto proj = glm::ortho(-scale, scale, -scale * width / height, scale * width / height, -1.0f, 1.0f);
            auto view = glm::translate(glm::mat4(1.f), redpos);

            shader.SetUniformMat4f("u_MVP", proj * view);
            // shader.SetUniform4f("u_Color", 1.0f, 0.f, 0.f, 1.f);
            renderer.Draw(va, ib, shader);

            auto speed = std::abs(relspeed * redpos.z);
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

            // if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            //     scale -= speed;

            // else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            //     scale += speed;

            // ImGui_ImplGlfw_NewFrame();
            // ImGui_ImplOpenGL3_NewFrame();
            // ImGui::NewFrame();
            // if (ImGui::BeginMainMenuBar())
            // {
            //     if (ImGui::BeginMenu("Load"))
            //     {
            //         if (/* condition */)
            //         {
            //             /* code */
            //         }
                    
            //         ImGui::EndMenu();
            //     }
                
            //     ImGui::EndMenuBar();
            // }
            // // ImGui::ShowDemoWindow();
            // ImGui::Render();
            // ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);

            auto frameTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count();
            title = "Hemicube;  FT: " + std::to_string(frameTime) + "ms; FPS: " + std::to_string(1000.0 / frameTime);
            glfwSetWindowTitle(window, title.c_str());

            glfwPollEvents();
        }
    }
    
    

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);

    glfwTerminate();
}
