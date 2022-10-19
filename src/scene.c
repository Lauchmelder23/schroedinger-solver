#include "scene.h"

#include <assert.h>
#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "objects/cube.h"

static void set_lighting_uniforms(Object* obj, Scene* scene)
{
	Shader shader = obj->shader;

	set_uniform_vec3(shader, "ambient_color", scene->ambient_light.color);
	set_uniform_float(shader, "ambient_intens", scene->ambient_light.intensity);

	set_uniform_vec3(shader, "point_pos", scene->point_light.position);
	set_uniform_vec3(shader, "point_col", scene->point_light.color);
	set_uniform_float(shader, "point_intens", scene->point_light.intensity);

	set_uniform_vec3(shader, "cam_pos", scene->camera.object.position);
}

int create_scene(Scene* scene)
{
	assert(scene);

	if (create_camera(&scene->camera) != 0)
	{
		fprintf(stderr, "Failed to set up camera for scene\n");
		return 1;
	}

	scene->objects = create_dynamic_array(Object*);
	scene->on_update = NULL;
	scene->last_frame_start = glfwGetTime();
}

void destroy_scene(Scene scene)
{
	for (size_t i = 0; i < scene.objects.size; i++)
	{
		Object* obj = *(Object**)dynamic_array_get(&scene.objects, i);
		destroy_object(*obj);
	}

	destroy_dynamic_array(scene.objects);
}

void scene_add_object(Scene* scene, Object* object)
{
	assert(scene);

	object->scene = scene;
	object->on_shader_use_scene = set_lighting_uniforms;
	dynamic_array_push(&scene->objects, (void*)&object);
}

void scene_update_size(Scene* scene, int width, int height)
{
	assert(scene);
	set_camera_perspective(&scene->camera, 90.0f, (float)width / (float)height);
}

void update_scene(Scene* scene)
{
	double frametime = glfwGetTime() - scene->last_frame_start;
	scene->last_frame_start = glfwGetTime();

	scene->on_update(scene, frametime);
}

void render_scene(Scene* scene)
{
	assert(scene);
	ctx_clear_screen(0.0f, 0.0f, 0.0f, 1.0f);

	for (size_t i = 0; i < scene->objects.size; i++)
	{
		Object* obj = *(Object**)dynamic_array_get(&scene->objects, i);
		update_object(obj);
		render_object(obj, &scene->camera);
	}
}
