#include "context.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "buffer.h"

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ctx_draw_elements(VertexArrayObject* vao)
{
	bind_vao(vao);
	glDrawElements(GL_TRIANGLES, vao->elements, GL_UNSIGNED_INT, (void*)0);
}

void ctx_enable(int feature)
{
	glEnable(feature);
}

void ctx_front_face(int front)
{
	glFrontFace(front);
}