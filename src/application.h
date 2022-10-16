#ifndef APPLICATION_H
#define APPLICATION_H

#include "renderer/window.h"
#include "renderer/buffer.h"
#include "renderer/shader.h"

typedef struct Application 
{
	GLFWwindow* window;

	VertexArrayObject object;
	int shader;
} Application;

int init_application(Application* app, const char* name);
int launch_application(Application* app);

#endif APPLICATION_H