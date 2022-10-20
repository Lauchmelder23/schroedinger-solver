#include "scene.h"

#include <assert.h>
#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "objects/cube.h"

int create_scene(Scene* scene)
{
	assert(scene);

	if (create_camera(&scene->camera) != 0)
	{
		fprintf(stderr, "Failed to set up camera for scene\n");
		return 1;
	}

	scene->on_update = NULL;
	scene->last_frame_start = glfwGetTime();
}

void destroy_scene(Scene scene)
{
	
}

void scene_update_size(Scene* scene, int width, int height)
{
	assert(scene);
	set_camera_perspective(&scene->camera, 90.0f, (float)width / (float)height);
}

void scene_set_camera_uniforms(Scene* scene, Shader* shader)
{
	set_uniform_mat4(shader, "view", get_camera_view(&scene->camera));
	set_uniform_mat4(shader, "projection", scene->camera.projection);
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

	scene->on_render(scene);
}
