
CPMAddPackage("gh:catchorg/Catch2@3.7.1")

add_executable(glube_tests
"fakegl.hpp"
"pooltests.cpp"
"tests.cpp"
)
target_compile_features(glube_tests PUBLIC cxx_std_23)
target_link_libraries(glube_tests PUBLIC glube Catch2::Catch2WithMain)
MJFullCompilerWarnings(glube_tests)

# list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
enable_testing()
add_test(glube_tests glube_tests)