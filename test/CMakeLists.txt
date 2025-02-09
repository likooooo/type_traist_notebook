set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(feature_testing feature_testing.cpp)

add_executable(test_cpp20 test.cpp)
target_compile_features(test_cpp20 PRIVATE cxx_std_20)
add_executable(test_cpp17 test.cpp)
target_compile_features(test_cpp17 PRIVATE cxx_std_17)