add_executable(glube_sample
"main.cpp"
)
target_compile_features(glube_sample PUBLIC cxx_std_23)
target_link_libraries(glube_sample PUBLIC glube)
MJFullCompilerWarnings(glube_sample)