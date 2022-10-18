#include "shader.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <glad/glad.h>

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

int create_shader(const char* vertex_shader_code, const char* fragment_shader_code)
{
	assert(vertex_shader_code && fragment_shader_code);

	int shader = glCreateProgram();

	char* error;
	int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	if (set_and_compile_shader(shader, vertex_shader, vertex_shader_code, &error) != 0)
	{
		fprintf(stderr, "vertex shader compilation failed: %s\n", error);
		free(error);
		glDeleteProgram(shader);
		shader = 0;

		goto vertex_failed;
	}

	int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	if (set_and_compile_shader(shader, fragment_shader, fragment_shader_code, &error) != 0)
	{
		fprintf(stderr, "fragment shader compilation failed: %s\n", error);
		free(error);
		glDeleteProgram(shader);
		shader = 0;

		goto fragment_failed;
	}

	glLinkProgram(shader);

	int result;
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
	{
		char error[512];
		glGetProgramInfoLog(shader, 512, NULL, error);

		fprintf(stderr, "shader linking failed: %s\n", error);
		glDeleteProgram(shader);
		shader = 0;
	}

fragment_failed:
	glDeleteShader(fragment_shader);
vertex_failed:
	glDeleteShader(vertex_shader);

	return shader;
}

void destroy_shader(int shader)
{
	glDeleteProgram(shader);
}

void bind_shader(int shader)
{
	glUseProgram(shader);
}

void set_uniform_mat4(int shader, const char* name, mat4 mat)
{
	int location = glGetUniformLocation(shader, name);
	glUniformMatrix4fv(location, 1, GL_FALSE, (float*)mat);
}