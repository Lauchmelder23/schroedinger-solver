# CMakeList.txt : CMake project for schroedinger-solver, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable(solver
	"main.c"
	"application.c"
	"renderer/window.c"
	"renderer/context.c"
)

target_link_libraries(solver PRIVATE
	glfw
	glad
)
