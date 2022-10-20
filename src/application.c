#include "application.h"

#include <assert.h>
#include <errno.h>

#include "renderer/context.h"
#include "objects/cube.h"
#include "scenes/spinning_cubes.h"

static void on_window_size_change(Application* app, int width, int height)
{
	scene_update_size(app->active_scene, width, height);
}

static void destroy_application(Application* app)
{
	assert(app);

	destroy_scene(*app->scenes);
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

	app->scenes = (Scene*)malloc(1 * sizeof(Scene));
	app->active_scene = app->scenes;	// First scene is active scene

	if (create_spinning_cubes_scene(&app->window, app->scenes) != 0)
	{
		return 1;
	}

	return 0;
}

int launch_application(Application* app) 
{
	while (!glfwWindowShouldClose(app->window.window))
	{
		glfwPollEvents();

		update_scene(app->active_scene);
		render_scene(app->active_scene);

		glfwSwapBuffers(app->window.window);
	}

	destroy_application(app);
}