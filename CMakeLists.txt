cmake_minimum_required(VERSION 3.29)

project(glube   VERSION 0.0.1
                DESCRIPTION "Cross platform C++ OpenGL wrapper library"
                LANGUAGES C CXX)

option(GLUBE_BUILD_TESTS "Build tests for glube")
option(GLUBE_BUILD_SAMPLE "Build sample for glube")

include("cmake/CPM.cmake")
CPMAddPackage("gh:glfw/glfw#3.4")
CPMAddPackage("gh:g-truc/glm#1.0.1")
CPMAddPackage("gh:MumblyJuergens/mj#main")
CPMAddPackage("gh:microsoft/GSL#main")

add_subdirectory("glad" SYSTEM)

set(glube_src
"src/glube.cpp"
"src/utilities.cpp"
"src/Window.cpp"
)
set(glube_headers
"include/glube/Attributes.hpp"
"include/glube/AutoBuffer.hpp"
"include/glube/BitmapText.hpp"
"include/glube/Buffer.hpp"
"include/glube/BuildingBuffer.hpp"
"include/glube/Context.hpp"
"include/glube/glube.hpp"
"include/glube/keys.hpp"
"include/glube/Program.hpp"
"include/glube/Sampler.hpp"
"include/glube/Shader.hpp"
"include/glube/Texture.hpp"
"include/glube/utilities.hpp"
"include/glube/Window.hpp"
"${CMAKE_CURRENT_BINARY_DIR}/include/glube/glube_export.h"
)
add_library(glube ${glube_src} ${glube_headers})
target_compile_features(glube PUBLIC cxx_std_23)
target_link_libraries(glube PUBLIC glad glfw glm::glm mj::mj Microsoft.GSL::GSL)
MJFullCompilerWarnings(glube)
MJLibrarySetup(glube INCLUDE_DIR "include" EXPORT_HEADER_PREFIX "glube/glube" HEADERS ${glube_headers})

if(${GLUBE_BUILD_TESTS} OR PROJECT_IS_TOP_LEVEL)
    add_subdirectory("tests")
endif()

if(${GLUBE_BUILD_SAMPLE} OR PROJECT_IS_TOP_LEVEL)
    add_subdirectory("sample")
endif()