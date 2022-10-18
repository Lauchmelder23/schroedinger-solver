#ifndef SHADER_H
#define SHADER_H

#include <cglm/mat4.h>

typedef int Shader;

Shader create_shader(const char* vertex_shader_code, const char* fragment_shader_code);
void destroy_shader(Shader shader);

void bind_shader(Shader shader);

void set_uniform_mat4(Shader shader, const char* name, mat4 mat);
void set_uniform_vec3(Shader shader, const char* name, vec3 vec);

#endif // SHADER_H