#ifndef OBJ_CUBE_H
#define OBJ_CUBE_H

#include "renderer/object.h"

typedef struct Cube {
	Object object;
	vec3 color;
	vec3 color_mod;
} Cube;

int create_cube(Cube* cube);
void destroy_cube(Cube* cube);

#endif // OBJ_CUBE_H