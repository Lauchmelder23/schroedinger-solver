#include "window.h"

#include <stdio.h>
#include <assert.h>

#include <GLFW/glfw3.h>

#include "context.h"

void default_framebuffer_size_callback(GLFWwindow* window, int vw, int vh) 
{
	ctx_viewport(0, 0, vw, vh);

	Window* managed = (Window*)glfwGetWindowUserPointer(window);
	if (managed->on_size_change)
	{
		managed->on_size_change(managed->user_data, vw, vh);
	}
}

int create_managed_window(Window* window, const char* title, int width, int height)
{
	assert(window);

	window->window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window == NULL) {
		const char* error;
		int error_code = glfwGetError(&error);

		fprintf(stderr, "Failed to create window (%d): %s\n", error_code, error);
		return error_code;
	}

	glfwMakeContextCurrent(window->window);
	ctx_init();
	ctx_enable(GL_DEPTH_TEST);
	ctx_front_face(GL_CW);

	glfwSetFramebufferSizeCallback(window->window, default_framebuffer_size_callback);
	
	glfwSetWindowUserPointer(window->window, (void*)window);
	window->on_size_change = NULL;

	return 0;
}

void destroy_window(Window window)
{
	glfwDestroyWindow(window.window);
}
