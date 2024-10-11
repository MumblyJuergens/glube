include(FetchContent)
FetchContent_Declare(Catch2 GIT_REPOSITORY "https://github.com/catchorg/Catch2.git" GIT_TAG "devel")
FetchContent_MakeAvailable(Catch2)

add_executable(glube_tests
"fakegl.hpp"
"pooltests.cpp"
"tests.cpp"
)
target_compile_features(glube_tests PUBLIC cxx_std_23)
target_link_libraries(glube_tests PUBLIC glube Catch2::Catch2WithMain)
target_compile_options(glube_tests PUBLIC ${glubecxxflags})

# list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(glube_tests)