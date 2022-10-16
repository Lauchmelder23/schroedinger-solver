#include "application.h"

#include <assert.h>
#include <errno.h>

#include <GLFW/glfw3.h>

#include "renderer/context.h"

int init_application(Application* app, const char* name)
{
	assert(app);
	glfwInit();

	app->window = create_managed_window(name, 800, 800);
	if (app->window == NULL) {
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
		glfwSwapBuffers(app->window);
	}
}