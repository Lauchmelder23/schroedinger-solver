#ifndef SHADER_H
#define SHADER_H

#include <cglm/mat4.h>

int create_shader(const char* vertex_shader_code, const char* fragment_shader_code);
void destroy_shader(int shader);

void bind_shader(int shader);
void set_uniform_mat4(int shader, const char* name, mat4 mat);

#endif // SHADER_H