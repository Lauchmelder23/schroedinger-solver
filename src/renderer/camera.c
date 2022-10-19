#include "camera.h"

#include <cglm/affine.h>
#include <cglm/cam.h>
#include <assert.h>

static const vec3 up = { 0.0f, 1.0f, 0.0f };

int create_camera(Camera* camera)
{
	assert(camera);

	init_object(&camera->object);

	glm_mat4_identity(camera->view);
	glm_perspective(glm_rad(90.0f), 1.0f, 0.001f, 100.0f, camera->projection);

	return 0;
}

vec4* get_camera_view(Camera* camera)
{
	glm_mat4_identity(camera->view);
	glm_lookat(camera->object.position, camera->look_at, up, camera->view);
	// glm_scale(camera->view, camera->object.scale);

	return camera->view;
}

void set_camera_perspective(Camera* camera, float fov, float ar)
{
	glm_perspective(glm_rad(fov), ar, 0.01f, 100.0f, camera->projection);
}
