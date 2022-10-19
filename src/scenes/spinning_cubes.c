#include "spinning_cubes.h"

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include <GLFW/glfw3.h>
#include <cglm/affine.h>

#include "objects/cube.h"
#include "application.h"

typedef struct SpinningCubes
{
	struct {
		double x, y;
	} last_cursor_pos;

	bool dragging;
	double drag_speed;
	double camera_distance;
} SpinningCubes;

static void init_spinning_cubes_data(SpinningCubes* scene)
{
	assert(scene);

	scene->camera_distance = 6.5f;
	scene->dragging = false;
	scene->drag_speed = 0.01f;
}

static void get_arcball_vector(vec3 dest, double width, double height, double x, double y)
{
	dest[0] = 1.0 * x / width * 2 - 1.0;
	dest[1] = 1.0 * y / height * 2 - 1.0;
	dest[2] = 0.0;

	dest[2] = -dest[2];
	float OP_squared = dest[0] * dest[0] + dest[1] * dest[1];
	if (OP_squared <= 1)
		dest[2] = sqrt(1 - OP_squared);
	else
		glm_normalize(dest);
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Application* app = glfwGetWindowUserPointer(window);
	Scene* scene = app->active_scene;
	SpinningCubes* data = scene->child;

	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
	{
		if (action == GLFW_PRESS)
		{
			data->dragging = true;

			glfwGetCursorPos(window, &data->last_cursor_pos.x, &data->last_cursor_pos.y);
		}
		else if (action == GLFW_RELEASE)
		{
			data->dragging = false;
		}
	} break;

	default:
		break;
	}
}

static void mouse_moved_callback(GLFWwindow* window, double xpos, double ypos)
{
	return;

	Application* app = glfwGetWindowUserPointer(window);
	Scene* scene = app->active_scene;
	SpinningCubes* data = scene->child;

	if (data->dragging && (xpos != data->last_cursor_pos.x || ypos != data->last_cursor_pos.y))
	{
		Object* camera = &scene->camera.object;
		int width, height;
		glfwGetWindowSize(window, &width, &height);

		vec3 va; get_arcball_vector(va, width, height, data->last_cursor_pos.x, data->last_cursor_pos.y);
		vec3 vb; get_arcball_vector(vb, width, height, xpos, ypos);

		vec3 diff; glm_vec3_sub(vb, va, diff);

		object_move(camera, diff);
		vec3 look_dir; glm_vec3_sub(scene->camera.look_at, camera->position, look_dir);
		double cam_distance = glm_vec3_norm(look_dir);
		if (cam_distance != data->camera_distance)
		{
			double correction = data->camera_distance - cam_distance;
			glm_normalize(look_dir);
			glm_vec3_scale(look_dir, correction, look_dir);

			object_move(camera, look_dir);
		}

		data->last_cursor_pos.x = xpos;
		data->last_cursor_pos.y = ypos;
	}
}

static void on_update(Scene* scene, double frametime)
{
}

void create_spinning_cubes_scene(Window* window, Scene* scene)
{
	assert(scene);

	create_scene(scene);
	scene->on_update = on_update;
	scene->child = malloc(sizeof(SpinningCubes));
	init_spinning_cubes_data(scene->child);

	for (int i = 0; i < 4; i++)
	{
		Cube* obj = (Cube*)malloc(sizeof(Cube));
		create_cube(obj);

		vec3 position = { 
			((i % 2) ? -1 : 1) * 1.5f, 
			0.0f, 
			((i < 2) ? -1 : 1) * 1.5 
		};
		object_set_position(&obj->object, position);

		scene_add_object(scene, &obj->object);
	}

	vec3 cam_pos = { 0.0f, 4.0f, -3.0 };
	object_set_position(&scene->camera.object, cam_pos);
	glm_vec3_zero(&scene->camera.look_at);

	glfwSetMouseButtonCallback(window->window, mouse_button_callback);
	glfwSetCursorPosCallback(window->window, mouse_moved_callback);
}
