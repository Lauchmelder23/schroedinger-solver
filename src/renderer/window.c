#include "window.h"

#include <stdio.h>
#include <GLFW/glfw3.h>

#include "context.h"

void default_framebuffer_size_callback(GLFWwindow* window, int vw, int vh) 
{
	ctx_viewport(0, 0, vw, vh);
}

GLFWwindow* create_managed_window(const char* title, int width, int height) 
{
	GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window == NULL) {
		const char* error;
		int error_code = glfwGetError(&error);

		fprintf(stderr, "Failed to create window (%d): %s\n", error_code, error);
		return NULL;
	}

	glfwMakeContextCurrent(window);
	ctx_init();

	glfwSetFramebufferSizeCallback(window, default_framebuffer_size_callback);

	return window;
}

void destroy_window(GLFWwindow* window)
{
	glfwDestroyWindow(window);
}
