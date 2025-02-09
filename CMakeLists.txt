cmake_minimum_required (VERSION 3.14)
project(type_traist_notebook 
    DESCRIPTION "type traist notebook"
    HOMEPAGE_URL "https://github.com/likooooo/type_traist_notebook"
    LANGUAGES CXX
)
add_subdirectory(test)

file(GLOB_RECURSE HEADERS "include/*.hpp" "include/*.h")
install(FILES ${HEADERS} DESTINATION include/type_traist_notebook)
