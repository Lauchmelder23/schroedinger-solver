#ifndef OBJECT_H
#define OBJECT_H

#include <cglm/mat4.h>

#include "shader.h"
#include "buffer.h"

typedef struct Camera Camera;

typedef struct Object
{
	VertexArrayObject* vao;
	Shader shader;

	mat4 transform;
	vec3 position;
	vec3 rotation;
	vec3 scale;

	void* child;
	void(*on_shader_use)(void*);
	void(*on_update)(void*);
	void(*on_destroy)(void*);
} Object;

void init_object(Object* obj);
void destroy_object(Object obj);

void object_set_position(Object* obj, vec3 position);
void object_move(Object* obj, vec3 direction);

void update_object(Object* obj);
void render_object(Object* obj, Camera* camera);

#endif // OBJECT_H