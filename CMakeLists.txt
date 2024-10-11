cmake_minimum_required(VERSION 3.30)

project(glube LANGUAGES C CXX)

include(FetchContent)
FetchContent_Declare(glfw GIT_REPOSITORY "https://github.com/glfw/glfw.git" GIT_TAG "master")
FetchContent_Declare(glm GIT_REPOSITORY "https://github.com/g-truc/glm.git" GIT_TAG "master")
FetchContent_Declare(ctre GIT_REPOSITORY "https://github.com/hanickadot/compile-time-regular-expressions.git" GIT_TAG "main")

FetchContent_MakeAvailable(glfw glm ctre)

if (MSVC)
    set(glubecxxflags ${glubecxxflags}
        /W4 # Baseline reasonable warnings
        /w14242 # 'identifier': conversion from 'type1' to 'type2', possible loss of data
        /w14254 # 'operator': conversion from 'type1:field_bits' to 'type2:field_bits', possible loss of data
        /w14263 # 'function': member function does not override any base class virtual member function
        /w14265 # 'classname': class has virtual functions, but destructor is not virtual instances of this class may not
                # be destructed correctly
        /w14287 # 'operator': unsigned/negative constant mismatch
        /we4289 # nonstandard extension used: 'variable': loop control variable declared in the for-loop is used outside
                # the for-loop scope
        /w14296 # 'operator': expression is always 'boolean_value'
        /w14311 # 'variable': pointer truncation from 'type1' to 'type2'
        /w14545 # expression before comma evaluates to a function which is missing an argument list
        /w14546 # function call before comma missing argument list
        /w14547 # 'operator': operator before comma has no effect; expected operator with side-effect
        /w14549 # 'operator': operator before comma has no effect; did you intend 'operator'?
        /w14555 # expression has no effect; expected expression with side- effect
        /w14619 # pragma warning: there is no warning number 'number'
        /w14640 # Enable warning on thread un-safe static member initialization
        /w14826 # Conversion from 'type1' to 'type2' is sign-extended. This may cause unexpected runtime behavior.
        /w14905 # wide string literal cast to 'LPSTR'
        /w14906 # string literal cast to 'LPWSTR'
        /w14928 # illegal copy-initialization; more than one user-defined conversion has been implicitly applied
        /permissive- # standards conformance mode for MSVC compiler.
        /WX
    )
else()
    set(glubecxxflags ${glubecxxflags}
        -Wall
        -Wextra # reasonable and standard
        -Wshadow # warn the user if a variable declaration shadows one from a parent context
        -Wnon-virtual-dtor # warn the user if a class with virtual functions has a non-virtual destructor. This helps catch hard to track down memory errors
        -Wold-style-cast # warn for c-style casts
        -Wcast-align # warn for potential performance problem casts
        -Wunused # warn on anything being unused
        -Woverloaded-virtual # warn if you overload (not override) a virtual function
        -Wpedantic # warn if non-standard C++ is used
        -Wconversion # warn on type conversions that may lose data
        -Wsign-conversion # warn on sign conversions
        -Wnull-dereference # warn if a null dereference is detected
        -Wdouble-promotion # warn if float is implicit promoted to double
        -Wformat=2 # warn on security issues around functions that format output (ie printf)
        -Wimplicit-fallthrough # warn on statements that fallthrough without an explicit annotation)
        -Werror
    )
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(glubecxxflags ${glubecxxflags}
            -Wmisleading-indentation # warn if indentation implies blocks where blocks do not exist
            -Wduplicated-cond # warn if if / else chain has duplicated conditions
            -Wduplicated-branches # warn if if / else branches have duplicated code
            -Wlogical-op # warn about logical operations being used where bitwise were probably wanted
            -Wuseless-cast # warn if you perform a cast to the same type
            -Wsuggest-override # warn if an overridden member function is not marked 'override' or 'final'
        )
    endif()
endif()

add_subdirectory("glad" SYSTEM)
add_subdirectory("tests")

add_library(glube)
target_compile_features(glube PUBLIC cxx_std_23)
target_link_libraries(glube PUBLIC glad glfw glm::glm ctre::ctre)
target_compile_options(glube PUBLIC ${glubecxxflags})
target_include_directories(glube PUBLIC "include")
target_sources(glube PUBLIC
"src/Context.cpp"
"src/glube.cpp"
"src/Window.cpp"
"include/glube/Attributes.hpp"
"include/glube/Buffer.hpp"
"include/glube/Context.hpp"
"include/glube/glube.hpp"
"include/glube/keys.hpp"
"include/glube/nameof.hpp"
"include/glube/Pool.hpp"
"include/glube/Program.hpp"
"include/glube/Shader.hpp"
"include/glube/Window.hpp"
)