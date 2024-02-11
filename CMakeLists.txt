cmake_minimum_required(VERSION 3.0.0)
project(hemicube VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

# set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)

include_directories("./dependencies/include")
link_directories("./dependencies/lib")
add_executable(hemicube src/main.cpp dependencies/src/glad.c)
target_link_libraries(hemicube PRIVATE glfw3 OpenGL::GL)

