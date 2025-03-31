cmake_minimum_required (VERSION 3.14)
project(type_traist_notebook 
    DESCRIPTION "type traist notebook"
    HOMEPAGE_URL "https://github.com/likooooo/type_traist_notebook"
    LANGUAGES CXX
)
add_compile_options(-Wno-c++23-extensions)
add_subdirectory(test)

file(GLOB_RECURSE HEADERS "include/*.hpp" "include/*.h")
set(PACKAGE_INCLUDE_INSTALL_DIR include/type_traist_notebook)
install(FILES ${HEADERS} DESTINATION ${PACKAGE_INCLUDE_INSTALL_DIR})

# 定义库版本
set(PACKAGE_VERSION "1.0.0")

# 创建并安装配置文件
configure_file(
    "${PROJECT_SOURCE_DIR}/cmake/type_traist_notebook-config.cmake.in"
    "${PROJECT_BINARY_DIR}/type_traist_notebook-config.cmake"
    @ONLY
)
install(FILES "${PROJECT_BINARY_DIR}/type_traist_notebook-config.cmake"
        DESTINATION lib/cmake/type_traist_notebook)

# 安装版本文件
configure_file(
    "${PROJECT_SOURCE_DIR}/cmake/type_traist_notebook-config-version.cmake.in"
    "${PROJECT_BINARY_DIR}/type_traist_notebook-config-version.cmake"
    @ONLY
)
install(FILES "${PROJECT_BINARY_DIR}/type_traist_notebook-config-version.cmake"
        DESTINATION lib/cmake/type_traist_notebook)