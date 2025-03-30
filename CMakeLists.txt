cmake_minimum_required(VERSION 3.16)

project(learnOpengl)

set(CMAKE_CXX_STANDARD 20)

set(GL_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/openglInclude")

list(APPEND LIBS "${CMAKE_SOURCE_DIR}/openglLink/glfw3.lib" "${CMAKE_SOURCE_DIR}/openglLink/assimp-vc143-mt.lib")

include_directories(${GL_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCES test.cpp glad.c)
add_executable(TestProj ${SOURCES})
target_link_libraries(TestProj ${LIBS})

set_target_properties(TestProj PROPERTIES 
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    # RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)