#ifndef AMBIENT_LIGHT_H
#define AMBIENT_LIGHT_H

#include <cglm/vec3.h>

typedef struct AmbientLight
{
	vec3 color;
	float intensity;
} AmbientLight;

#endif