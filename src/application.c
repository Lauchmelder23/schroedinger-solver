#include "application.h"

#include <assert.h>
#include <errno.h>

#include <GLFW/glfw3.h>
#include <cglm/affine.h>

#include "renderer/context.h"

static void on_window_size_change(Application* app, int width, int height)
{
	set_camera_perspective(&app->camera, 90.0f, (float)width / (float)height);
}

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

	if (create_managed_window(&app->window, name, 800, 800) != 0)
	{
		destroy_application(app);
		return 1;
	}

	app->window.user_data = app;
	app->window.on_size_change = on_window_size_change;

	// Create quad for testing
	create_vao(&app->object);

	float vertices[] = {
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,	// front bottom left
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,	// front top left
		 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f,	// front top right
		 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,	// front bottom right

		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,	// back bottom left
		-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,	// back top left
		 0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,	// back top right
		 0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,	// back bottom right
	};
	attach_vertex_buffer(&app->object, vertices, sizeof(vertices));

	unsigned indices[] = {
		0, 1, 2,	// front
		0, 2, 3,

		3, 2, 6,	// right
		3, 6, 7,
		
		4, 5, 1,	// left
		4, 1, 0,

		7, 6, 5,	// back
		7, 5, 4,

		1, 5, 6,	// top
		1, 6, 2,

		4, 0, 3,	// bottom	
		4, 3, 7
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
		"uniform mat4 model;"
		"uniform mat4 view;"
		"uniform mat4 projection;"
		""
		"void main() {"
		"	i_col = col;"
		"	gl_Position = projection * view * model * vec4(pos, 1.0);"
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

	if (create_camera(&app->camera) != 0)
	{
		destroy_application(app);
		return 1;
	}

	glm_mat4_identity(app->model);

	ctx_enable(GL_DEPTH_TEST);
	ctx_front_face(GL_CW);

	return 0;
}

int launch_application(Application* app) 
{
	while (!glfwWindowShouldClose(app->window.window))
	{
		glfwPollEvents();

		// rotate cube
		glm_rotate(app->model, 0.03f, (vec3) { 0.0f, 1.0f, 0.0f });

		ctx_clear_screen(0.3f, 0.1f, 0.8f, 1.0f);

		bind_shader(app->shader);
		set_uniform_mat4(app->shader, "model", app->model);
		set_uniform_mat4(app->shader, "view", app->camera.view);
		set_uniform_mat4(app->shader, "projection", app->camera.projection);

		ctx_draw_elements(&app->object);

		glfwSwapBuffers(app->window.window);
	}

	destroy_application(app);
}