#ifndef SHADER_H
#define SHADER_H

#include <cglm/mat4.h>
#include "util/dynarray.h"

typedef struct Object Object;

typedef struct Shader
{
	int id;
	DynamicArray objects;
} Shader;

int create_shader(Shader* shader, const char* vertex_shader_code, const char* fragment_shader_code);
void destroy_shader(Shader shader);

void bind_shader(Shader* shader);
void shader_add_object(Shader* shader, Object* object);
void shader_render(Shader* shader);

void set_uniform_mat4(Shader* shader, const char* name, mat4 mat);
void set_uniform_float(Shader* shader, const char* name, float val);
void set_uniform_vec3(Shader* shader, const char* name, vec3 vec);

#endif // SHADER_H