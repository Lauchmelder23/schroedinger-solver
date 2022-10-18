#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/mat4.h>

typedef struct Camera
{
	mat4 view;
	mat4 projection;
} Camera;

int create_camera(Camera* camera);
void set_camera_perspective(Camera* camera, float fov, float ar);

#endif // CAMERA_H