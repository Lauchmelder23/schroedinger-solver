#ifndef SHADER_H
#define SHADER_H

int create_shader(const char* vertex_shader_code, const char* fragment_shader_code);
void destroy_shader(int shader);

void bind_shader(int shader);

#endif // SHADER_H