#include "shader.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

#include <glad/glad.h>

#include "object.h"

static int set_and_compile_shader(GLuint program, GLuint shader, const char* code, char** error)
{
	int result;

	glShaderSource(shader, 1, &code, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) 
	{
		*error = (char*)malloc(1024);
		if (*error == NULL) {
			fprintf(stderr, "(shader) memory allocation for error buffer failed\n");
			return -ENOMEM;
		}

		glGetShaderInfoLog(shader, 1024, NULL, *error);
		(*error)[1023] = NULL;
		return 1;
	}

	glAttachShader(program, shader);

	return 0;
}

int create_shader(Shader* shader, const char* vertex_shader_code, const char* fragment_shader_code)
{
	assert(shader);
	assert(vertex_shader_code && fragment_shader_code);

	int return_val = 0;

	shader->objects = create_dynamic_array(Object*);
	shader->id = glCreateProgram();

	char* error;
	int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	if (set_and_compile_shader(shader->id, vertex_shader, vertex_shader_code, &error) != 0)
	{
		fprintf(stderr, "vertex shader compilation failed: %s\n", error);
		free(error);
		glDeleteProgram(shader->id);
		shader->id = 0;
		return_val = 1;

		goto vertex_failed;
	}

	int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	if (set_and_compile_shader(shader->id, fragment_shader, fragment_shader_code, &error) != 0)
	{
		fprintf(stderr, "fragment shader compilation failed: %s\n", error);
		free(error);
		glDeleteProgram(shader->id);
		shader->id = 0;
		return_val = 1;

		goto fragment_failed;
	}

	glLinkProgram(shader->id);

	int result;
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
	{
		char error[512];
		glGetProgramInfoLog(shader->id, 512, NULL, error);

		fprintf(stderr, "shader linking failed: %s\n", error);
		glDeleteProgram(shader);
		shader->id = 0;
		return_val = 1;
	}

fragment_failed:
	glDeleteShader(fragment_shader);
vertex_failed:
	glDeleteShader(vertex_shader);

	return return_val;
}

void destroy_shader(Shader shader)
{
	destroy_dynamic_array(shader.objects);
	glDeleteProgram(shader.id);
}

void bind_shader(Shader* shader)
{
	dynamic_array_clear(&shader->objects);
	glUseProgram(shader->id);
}

void shader_add_object(Shader* shader, Object* object)
{
	dynamic_array_push(&shader->objects, (void*)&object);
}

void shader_render(Shader* shader)
{
	for (int i = 0; i < shader->objects.size; i++)
	{
		Object* obj = *(Object**)dynamic_array_get(&shader->objects, i);
		render_object(obj, shader);
	}
}

void set_uniform_mat4(Shader* shader, const char* name, mat4 mat)
{
	int location = glGetUniformLocation(shader->id, name);
	glUniformMatrix4fv(location, 1, GL_FALSE, (float*)mat);
}

void set_uniform_float(Shader* shader, const char* name, float val)
{
	int location = glGetUniformLocation(shader->id, name);
	glUniform1f(location, val);
}

void set_uniform_vec3(Shader* shader, const char* name, vec3 vec)
{
	int location = glGetUniformLocation(shader->id, name);
	glUniform3fv(location, 1, vec);
}
