#ifndef APPLICATION_H
#define APPLICATION_H

#include "renderer/window.h"

typedef struct Application 
{
	GLFWwindow* window;
} Application;

int init_application(Application* app, const char* name);
int launch_application(Application* app);

#endif APPLICATION_H