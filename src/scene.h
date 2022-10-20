#ifndef SCENE_H
#define SCENE_H

#include "renderer/camera.h"
#include "renderer/object.h"
#include "util/dynarray.h"

#include "renderer/light/ambient_light.h"
#include "renderer/light/point_light.h"

typedef struct Scene
{
	Camera camera;

	void* child;
	void(*on_update)(void*, double);
	void(*on_render)(void*);
	double last_frame_start;
} Scene;

int create_scene(Scene* scene);
void destroy_scene(Scene scene);

void scene_update_size(Scene* scene, int width, int height);
void scene_set_camera_uniforms(Scene* scene, Shader* shader);

void update_scene(Scene* scene);
void render_scene(Scene* scene);

#endif