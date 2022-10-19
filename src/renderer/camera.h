#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/mat4.h>
#include "object.h"

typedef struct Camera
{
	Object object;
	vec3 look_at;

	mat4 view;
	mat4 projection;
} Camera;

int create_camera(Camera* camera);

vec4* get_camera_view(Camera* camera);
void set_camera_perspective(Camera* camera, float fov, float ar);

#endif // CAMERA_H