#ifndef APPLICATION_H
#define APPLICATION_H

#include "renderer/window.h"
#include "scene.h"

typedef struct Application 
{
	Window window;

	Scene* scenes;
	Scene* active_scene;
} Application;

int init_application(Application* app, const char* name);
int launch_application(Application* app);

#endif APPLICATION_H