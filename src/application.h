#ifndef APPLICATION_H
#define APPLICATION_H

#include "renderer/window.h"
#include "renderer/buffer.h"
#include "renderer/shader.h"
#include "renderer/camera.h"

typedef struct Application 
{
	Window window;

	VertexArrayObject object;
	int shader;
	Camera camera;
	mat4 model;
} Application;

int init_application(Application* app, const char* name);
int launch_application(Application* app);

#endif APPLICATION_H