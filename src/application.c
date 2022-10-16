#include "application.h"

#include <assert.h>
#include <errno.h>

#include <GLFW/glfw3.h>

#include "renderer/context.h"

static void destroy_application(Application* app)
{
	assert(app);

	destroy_vao(app->object);
	destroy_window(app->window);
}

int init_application(Application* app, const char* name)
{
	assert(app);
	glfwInit();

	app->window = create_managed_window(name, 800, 800);
	if (app->window == NULL) {
		return 1;
	}

	// Create quad for testing
	create_vao(&app->object);

	float vertices[] = {
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f
	};
	attach_vertex_buffer(&app->object, vertices, sizeof(vertices));

	unsigned indices[] = {
		0, 1, 2,
		0, 2, 3
	};
	attach_element_buffer(&app->object, indices, sizeof(indices));

	VertexAttribute layout[] = {
		{ GL_FLOAT, 3, 3 * sizeof(float) },
		{ GL_FLOAT, 3, 3 * sizeof(float) }
	};
	set_vertex_layout(&app->object, layout, sizeof(layout) / sizeof(VertexAttribute));

	app->shader = create_shader(
		"#version 460 core\n"
		""
		"layout (location = 0) in vec3 pos;"
		"layout (location = 1) in vec3 col;"
		""
		"out vec3 i_col;"
		""
		"void main() {"
		"	i_col = col;"
		"	gl_Position = vec4(pos, 1.0);"
		"}",

		"#version 460 core\n"
		""
		"in  vec3 i_col;"
		"out vec4 FragColor;"
		""
		"void main() {"
		"	FragColor = vec4(i_col, 1.0);"
		"}"
	);
	if(app->shader == 0)
	{
		destroy_application(app);
		return 1;
	}

	return 0;
}

int launch_application(Application* app) 
{
	while (!glfwWindowShouldClose(app->window))
	{
		glfwPollEvents();

		ctx_clear_screen(0.3f, 0.1f, 0.8f, 1.0f);

		bind_shader(app->shader);
		ctx_draw_elements(&app->object);

		glfwSwapBuffers(app->window);
	}

	destroy_application(app);
}