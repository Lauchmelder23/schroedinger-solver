#include "context.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

int ctx_init() 
{
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

void ctx_viewport(int x, int y, int w, int h)
{
	glViewport(x, y, w, h);
}

void ctx_clear_screen(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);
}
