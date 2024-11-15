cmake_minimum_required(VERSION 3.29)

project(glube   VERSION 0.0.1
                DESCRIPTION "Cross platform C++ OpenGL wrapper library"
                LANGUAGES C CXX)

option(GLUBE_BUILD_TESTS "Build tests for glube")

include("cmake/CPM.cmake")
CPMAddPackage("gh:glfw/glfw#3.4")
CPMAddPackage("gh:g-truc/glm#1.0.1")
CPMAddPackage("gh:MumblyJuergens/mj#main")

add_subdirectory("glad" SYSTEM)

set(glube_src
"src/glube.cpp"
"src/Window.cpp"
)
set(glube_headers
"include/glube/Attributes.hpp"
"include/glube/Buffer.hpp"
"include/glube/Context.hpp"
"include/glube/glube.hpp"
"include/glube/keys.hpp"
"include/glube/Program.hpp"
"include/glube/Shader.hpp"
"include/glube/Window.hpp"
"${CMAKE_CURRENT_BINARY_DIR}/include/glube/glube_export.h"
)
add_library(glube ${glube_src} ${glube_headers})
target_compile_features(glube PUBLIC cxx_std_23)
target_link_libraries(glube PUBLIC glad glfw glm::glm mj::mj)
MJFullCompilerWarnings(glube)
MJLibrarySetup(glube INCLUDE_DIR "include" EXPORT_HEADER_PREFIX "glube/glube" HEADERS ${glube_headers})

if(PROJECT_IS_TOP_LEVEL)
    MJLibraryInstallSetup(glube)
    add_subdirectory("tests")
endif()