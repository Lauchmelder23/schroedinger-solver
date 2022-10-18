#include "object.h"

#include <cglm/affine.h>
#include "context.h"

void init_object(Object* obj)
{
	glm_mat4_identity(obj->transform);
	glm_vec3_zero(obj->position);
	glm_vec3_zero(obj->rotation);
	glm_vec3_one(obj->scale);
}

void update_object(Object* obj)
{
	obj->on_update(obj->child);
}

void render_object(Object* obj, Camera* camera)
{
	bind_shader(obj->shader);

	glm_mat4_identity(obj->transform);
	float angle = glm_vec3_norm(obj->rotation);
	vec3 normalized_rot;
	glm_vec3_normalize_to(obj->rotation, normalized_rot);

	glm_scale(obj->transform, obj->scale);
	glm_rotate(obj->transform, angle, normalized_rot);
	glm_scale(obj->transform, obj->scale);

	set_uniform_mat4(obj->shader, "model", obj->transform);
	set_uniform_mat4(obj->shader, "view", camera->view);
	set_uniform_mat4(obj->shader, "projection", camera->projection);

	obj->on_shader_use(obj->child);

	ctx_draw_elements(obj->vao);
}

void destroy_object(Object obj)
{
	obj.on_destroy(obj.child);
}
