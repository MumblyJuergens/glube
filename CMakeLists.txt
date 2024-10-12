cmake_minimum_required(VERSION 3.29)

project(glube LANGUAGES C CXX)

option(GLUBE_BUILD_TESTS "Build tests for glube")

include("cmake/CPM.cmake")
CPMAddPackage("gh:glfw/glfw#3.4")
CPMAddPackage("gh:g-truc/glm#1.0.1")
CPMAddPackage("gh:MumblyJuergens/mj#main")

add_subdirectory("glad" SYSTEM)

add_library(glube STATIC 
"src/Context.cpp"
"src/glube.cpp"
"src/Window.cpp"
"include/glube/Attributes.hpp"
"include/glube/Buffer.hpp"
"include/glube/Context.hpp"
"include/glube/glube.hpp"
"include/glube/keys.hpp"
"include/glube/Program.hpp"
"include/glube/Shader.hpp"
"include/glube/Window.hpp"
)
target_compile_features(glube PUBLIC cxx_std_23)
target_link_libraries(glube PUBLIC glad glfw glm::glm mj::mj)
MJFullCompilerWarnings(glube)
target_include_directories(glube PUBLIC "include")

if(${GLUBE_BUILD_TESTS} OR PROJECT_IS_TOP_LEVEL)
    add_subdirectory("tests")
endif()