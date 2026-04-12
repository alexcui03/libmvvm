cmake_minimum_required(VERSION 3.30)

project(libmvvm VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(mvvm INTERFACE)
target_compile_options(mvvm INTERFACE -Wall -Wextra -O2 -freflection)
target_compile_features(mvvm INTERFACE cxx_std_26)
target_include_directories(mvvm INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Examples
add_subdirectory(example)
