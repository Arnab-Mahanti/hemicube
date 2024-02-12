cmake_minimum_required(VERSION 3.0.0)
project(hemicube VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

# set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)

include_directories("./dependencies/include/")
link_directories("./dependencies/lib/")

file(GLOB_RECURSE HEMICUBE_SRC "src/*.cpp" "src/*.h" "dependencies/src/*.c" "dependencies/src/*.cpp")

include_directories(src)
add_executable(hemicube ${HEMICUBE_SRC})
target_link_libraries(hemicube PRIVATE glfw3 OpenGL::GL)

