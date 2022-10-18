#ifndef SCENE_H
#define SCENE_H

#include "renderer/camera.h"
#include "renderer/object.h"

typedef struct Scene
{
	Camera camera;
	Object* objects;
} Scene;

int create_scene(Scene* scene);
void destroy_scene(Scene scene);

void scene_add_object(Scene* scene, Object* object);
void scene_update_size(Scene* scene, int width, int height);

void render_scene(Scene* scene);

#endif