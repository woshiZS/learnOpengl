cmake_minimum_required(VERSION 3.16)

project(learnOpengl)

set(CMAKE_CXX_STANDARD 20)

list(APPEND GL_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/openglInclude" "common")

list(APPEND LIBS "${CMAKE_SOURCE_DIR}/openglLink/glfw3.lib" "${CMAKE_SOURCE_DIR}/openglLink/assimp-vc143-mt.lib")

include_directories(${GL_INCLUDE_DIRS})

file(GLOB_RECURSE COMMON_FILES "common/*.h" "common/*.cpp" "common/*.c")

add_subdirectory(TestPlayGround)