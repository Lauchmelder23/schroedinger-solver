# CMakeList.txt : CMake project for schroedinger-solver, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable(solver
	"main.c"
	"application.c"
	"scene.c"
	"renderer/window.c"
	"renderer/context.c"
	"renderer/buffer.c" 
	"renderer/shader.c"
	"renderer/camera.c"
  "renderer/object.c" "util/dynarray.h" "util/dynarray.c" "scenes/test_scene.h" "scenes/test_scene.c" "renderer/light/point_light.h" "renderer/light/ambient_light.h" "objects/arrow.h" "objects/arrow.c")

target_include_directories(solver PRIVATE
	${CMAKE_CURRENT_LIST_DIR}
)


target_link_libraries(solver PRIVATE
	glfw
	glad
	cglm
)
