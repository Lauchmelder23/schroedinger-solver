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
	DynamicArray objects;

	AmbientLight ambient_light;
	PointLight point_light;

	void* child;
	void(*on_update)(void*, double);
	double last_frame_start;
} Scene;

int create_scene(Scene* scene);
void destroy_scene(Scene scene);

void scene_add_object(Scene* scene, Object* object);
void scene_update_size(Scene* scene, int width, int height);

void update_scene(Scene* scene);
void render_scene(Scene* scene);

#endif