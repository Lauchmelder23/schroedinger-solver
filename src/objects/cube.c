#include "cube.h"

#include <stdio.h>
#include <glad/glad.h>

static VertexArrayObject* vao = NULL;
static Shader shader = 0;
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
		-1.0f, -1.0f, 1.0f,
		-1.0f,  1.0f, 1.0f,
		 1.0f,  1.0f, 1.0f,
		 1.0f, -1.0f, 1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
	};
	attach_vertex_buffer(vao, cube_vertices, sizeof(cube_vertices));

	unsigned cube_indices[] = {
		0, 1, 2, 2, 1, 3,
		1, 5, 6, 1, 6, 2,
		3, 2, 6, 3, 6, 7,
		4, 5, 1, 4, 1, 0,
		4, 0, 3, 4, 3, 7,
		7, 6, 5, 7, 5, 4
	};
	attach_element_buffer(vao, cube_indices, sizeof(cube_indices));

	VertexAttribute attributes[] = {
		{ GL_FLOAT, 3, sizeof(float) }
	};

	set_vertex_layout(vao, attributes, sizeof(attributes) / sizeof(VertexAttribute));

	shader = create_shader(
		"#version 460 core\n"
		""
		"layout (location = 0) in vec3 pos;"
		""
		"uniform mat4 model;"
		"uniform mat4 view;"
		"uniform mat4 projection;"
		""
		"void main() {"
		"	gl_Position = projection * view * model * vec4(pos, 1.0);"
		"}",

		"#version 460 core\n"
		""
		"uniform vec3 cube_color;"
		"out vec4 FragColor;"
		""
		"void main() {"
		"	FragColor = vec4(cube_color, 1.0);"
		"}"
	);
	if (shader == 0) 
	{ 
		fprintf(stderr, "failed to create shader for cube\n");
		return 1;
	}

	return 0;
}

static void on_shader_bind(Cube* cube)
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
	if (vao == NULL || shader == 0)
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
	cube->object.shader = shader;

	cube->object.child = cube;
	cube->object.on_shader_use = on_shader_bind;
	cube->object.on_update = on_update;
	cube->object.on_destroy = destroy_cube;

	num_cubes++;
}

void destroy_cube(Cube* cube)
{
	num_cubes--;

	if (num_cubes == 0)
	{
		destroy_shader(shader);
		shader = 0;

		destroy_vao(*vao);
		vao = NULL;
	}
}

