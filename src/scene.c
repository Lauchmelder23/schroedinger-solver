#include "scene.h"

#include <assert.h>
#include <stdio.h>

#include <glad/glad.h>

#include "objects/cube.h"

int create_scene(Scene* scene)
{
	assert(scene);

	if (create_camera(&scene->camera) != 0)
	{
		fprintf(stderr, "Failed to set up camera for scene\n");
		return 1;
	}

	scene->objects = (Object*)malloc(sizeof(Object));
}

void destroy_scene(Scene scene)
{
	destroy_object(*(scene.objects));
}

void scene_add_object(Scene* scene, Object* object)
{
	assert(scene);

	scene->objects = object;
}

void scene_update_size(Scene* scene, int width, int height)
{
	assert(scene);
	set_camera_perspective(&scene->camera, 90.0f, (float)width / (float)height);
}

void render_scene(Scene* scene)
{
	assert(scene);
	ctx_clear_screen(0.0f, 0.0f, 0.0f, 1.0f);

	update_object(scene->objects);
	render_object(scene->objects, &scene->camera);
}
