#include "object.h"

#include <cglm/affine.h>
#include "context.h"
#include "camera.h"

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

	glm_translate(obj->transform, obj->position);
	glm_rotate(obj->transform, angle, normalized_rot);
	glm_scale(obj->transform, obj->scale);

	set_uniform_mat4(obj->shader, "model", obj->transform);
	set_uniform_mat4(obj->shader, "view", get_camera_view(camera));
	set_uniform_mat4(obj->shader, "projection", camera->projection);

	obj->on_shader_use_obj(obj->child);
	obj->on_shader_use_scene(obj, obj->scene);

	ctx_draw_elements(obj->vao);
}

void destroy_object(Object obj)
{
	obj.on_destroy(obj.child);
}

void object_set_position(Object* obj, vec3 position)
{
	glm_vec3_copy(position, obj->position);
}

void object_move(Object* obj, vec3 direction)
{
	glm_vec3_add(obj->position, direction, obj->position);
}
