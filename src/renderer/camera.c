#include "camera.h"

#include <cglm/affine.h>
#include <cglm/cam.h>
#include <assert.h>

int create_camera(Camera* camera)
{
	assert(camera);

	glm_mat4_identity(camera->view);
	glm_rotate(camera->view, glm_rad(35.0f), (vec3) { 1.0f, 0.0f, 0.0f });
	glm_translate(camera->view, (vec3) { 0.0f, -1.0f, -2.0f });

	glm_perspective(glm_rad(90.0f), 1.0f, 0.001f, 100.0f, camera->projection);

	return 0;
}

void set_camera_perspective(Camera* camera, float fov, float ar)
{
	glm_perspective(glm_rad(fov), ar, 0.01f, 100.0f, camera->projection);
}
