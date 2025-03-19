set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(feature_testing feature_testing.cpp)

file(GLOB tests ${CMAKE_CURRENT_LIST_DIR}/test_*.cpp)
foreach(test IN LISTS tests)
    get_filename_component(test_WLE ${test} NAME_WLE)
    set(test_name ${test_WLE}_cpp20)
    add_executable(${test_name} ${test})
    target_compile_features(${test_name} PRIVATE cxx_std_20)
    
    set(test_name ${test_WLE}_cpp17)
    add_executable(${test_name} ${test})
    target_compile_features(${test_name} PRIVATE cxx_std_17)
endforeach()