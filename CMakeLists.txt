# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("schroedinger-solver")

add_subdirectory("vendor/glfw")
add_subdirectory("vendor/cglm")
add_subdirectory("vendor/glad")

# Include sub-projects.
add_subdirectory ("src")
