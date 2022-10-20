#include "cube.h"

#include <stdio.h>
#include <glad/glad.h>

static VertexArrayObject* vao = NULL;
static size_t num_cubes = 0;

static int init_cube_object(void)
{
	vao = (VertexArrayObject*) malloc(sizeof(VertexArrayObject));
	if (vao == NULL) 
	{
		fprintf(stderr, "Failed to allocate memory for cube vao\n");
		return 1;
	}

	create_vao(vao);
	float cube_vertices[] = {
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		-1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,

		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
		-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
		-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,

		 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
		 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
		 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
		 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
		 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
		 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,

		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,
		 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,
		 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
		 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
		-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,

		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f
	};
	attach_vertex_buffer(vao, cube_vertices, sizeof(cube_vertices));

	unsigned cube_indices[] = {
		0, 1, 2, 3, 4, 5,
		6, 7, 8, 9, 10, 11,
		12, 13, 14, 15, 16, 17,
		18, 19, 20, 21, 22, 23,
		24, 25, 26, 27, 28, 29,
		30, 31, 32, 33, 34, 35
	};
	attach_element_buffer(vao, cube_indices, sizeof(cube_indices));

	VertexAttribute attributes[] = {
		{ GL_FLOAT, 3, sizeof(float) },
		{ GL_FLOAT, 3, sizeof(float) }
	};
	set_vertex_layout(vao, attributes, sizeof(attributes) / sizeof(VertexAttribute));

	return 0;
}

static void on_shader_bind(Cube* cube, Shader* shader)
{
	set_uniform_vec3(shader, "cube_color", cube->color);
}

static void on_update(Cube* cube)
{
	cube->object.rotation[1] += 0.05f;

	for (int i = 0; i < 3; i++) 
	{
		cube->color[i] += cube->color_mod[i];

		if (cube->color[i] > 1.0f) 
		{
			cube->color_mod[i] *= -1.0f;
			cube->color[i] = 1.0f;
		}
		else if (cube->color[i] < 0.0f)
		{
			cube->color_mod[i] *= -1.0f;
			cube->color[i] = 0.0f;
		}
	}
}

int create_cube(Cube* cube)
{
	if (vao == NULL)
	{
		if (init_cube_object() != 0)
		{
			return 1;
		}
	}

	init_object(&cube->object);
	glm_vec3_one(cube->color);

	cube->color_mod[0] = 0.01f;
	cube->color_mod[1] = 0.006f;
	cube->color_mod[2] = 0.005f;

	cube->object.vao = vao;

	cube->object.child = cube;
	cube->object.on_shader_use_obj = on_shader_bind;
	cube->object.on_update = on_update;
	cube->object.on_destroy = destroy_cube;

	num_cubes++;
}

void destroy_cube(Cube* cube)
{
	num_cubes--;

	if (num_cubes == 0)
	{
		destroy_vao(*vao);
		vao = NULL;
	}
}


