cmake_minimum_required(VERSION 3.0.0)
project(rev.ng VERSION 0.1.0 LANGUAGES CXX)

# Find dependencies provided by vcpkg (via vcpkg.cmake)

set(CMAKE_CXX_STANDARD 20)

enable_testing()

add_subdirectory(compiler)
add_subdirectory(tests)
