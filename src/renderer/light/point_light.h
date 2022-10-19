#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include <cglm/vec3.h>

typedef struct PointLight
{
	vec3 color;
	vec3 position;
	float intensity;
} PointLight;

#endif