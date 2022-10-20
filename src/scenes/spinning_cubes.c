#include "spinning_cubes.h"

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include <GLFW/glfw3.h>
#include <cglm/affine.h>

#include "objects/cube.h"
#include "application.h"
#include "renderer/object.h"

static const char* vertex_shader = 
"#version 460 core\n"
""
"layout (location = 0) in vec3 pos;"
"layout (location = 1) in vec3 normal;"
""
"out vec3 frag_normal;"
"out vec3 frag_pos;"
""
"uniform mat4 model;"
"uniform mat4 view;"
"uniform mat4 projection;"
""
"void main() {"
"	frag_normal = mat3(transpose(inverse(model))) * normal;"
"	frag_pos = vec3(model * vec4(pos, 1.0));"
"	gl_Position = projection * view * model * vec4(pos, 1.0);"
"}";

static const char* fragment_shader =
"#version 460 core\n"
""
"in vec3 frag_pos;"
"in vec3 frag_normal;"
""
"uniform vec3 ambient_color = vec3(1.0, 1.0, 1.0);"
"uniform float ambient_intens = 1.0f;"
""
"uniform vec3 point_pos = vec3(0.0, 0.0, 0.0);"
"uniform vec3 point_col = vec3(1.0, 1.0, 1.0);"
"uniform float point_intens = 1.0f;"
""
"uniform vec3 cam_pos = vec3(0.0, 0.0, 0.0);"
"float specular_strength = 0.5f;"
""
"uniform vec3 cube_color;"
""
"out vec4 FragColor;"
""
"void main() {"
"	vec3 ambient = ambient_intens * ambient_color;"
""
"	vec3 norm = normalize(frag_normal);"
"	vec3 light_dir = normalize(point_pos - frag_pos);"
"	float diff = max(dot(norm, light_dir), 0.0);"
"	vec3 diffuse = diff * point_col;"
""
"	vec3 view_dir = normalize(cam_pos - frag_pos);"
"	vec3 reflect_dir = reflect(-light_dir, norm);"
""
"	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);"
"	vec3 specular = specular_strength * spec * point_col;  "
""
"	vec3 result = (ambient + diffuse + specular) * cube_color;"
"	FragColor = vec4(result, 1.0);"
"}";

typedef struct SpinningCubes
{
	Shader cube_shader;
	DynamicArray objects;

	AmbientLight ambient_light;
	PointLight point_light;

	struct {
		double x, y;
	} last_cursor_pos;

	bool dragging;
	double drag_speed;
	double camera_distance;

	double time;
} SpinningCubes;

static void set_lighting_uniforms(Shader* shader, Scene* scene)
{
	SpinningCubes* data = scene->child;

	set_uniform_vec3(shader, "ambient_color", data->ambient_light.color);
	set_uniform_float(shader, "ambient_intens", data->ambient_light.intensity);

	set_uniform_vec3(shader, "point_pos", data->point_light.position);
	set_uniform_vec3(shader, "point_col", data->point_light.color);
	set_uniform_float(shader, "point_intens", data->point_light.intensity);

	set_uniform_vec3(shader, "cam_pos", scene->camera.object.position);
}

static void init_spinning_cubes_data(SpinningCubes* scene)
{
	assert(scene);

	scene->camera_distance = 6.5f;
	scene->dragging = false;
	scene->drag_speed = 0.01f;
	scene->time = 0.0f;

	scene->objects = create_dynamic_array(Object*);
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
	SpinningCubes* data = scene->child;

	for (int i = 0; i < data->objects.size; i++)
	{
		Object* obj = *(Object**)dynamic_array_get(&data->objects, i);
		update_object(obj);
	}

	vec3 cam_pos = { 6.0f * sin(2.0f * data->time), 3.5f * cos(data->time), 6.0f * cos(2.0f * data->time)};

	object_set_position(&scene->camera.object, cam_pos);
	data->time += frametime;

	glm_vec3_copy(scene->camera.object.position, data->point_light.position);
}

static void on_render(Scene* scene)
{
	SpinningCubes* data = scene->child;

	bind_shader(&data->cube_shader);
	set_lighting_uniforms(&data->cube_shader, scene);
	scene_set_camera_uniforms(scene, &data->cube_shader);

	for (int i = 0; i < data->objects.size; i++)
	{
		shader_add_object(&data->cube_shader, *(Object**)dynamic_array_get(&data->objects, i));
	}

	shader_render(&data->cube_shader);
}

int create_spinning_cubes_scene(Window* window, Scene* scene)
{
	assert(scene);

	create_scene(scene);
	scene->on_update = on_update;
	scene->on_render = on_render;
	scene->child = malloc(sizeof(SpinningCubes));
	init_spinning_cubes_data(scene->child);

	SpinningCubes* data = scene->child;
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

		Object* raw_obj = &obj->object;
		dynamic_array_push(&data->objects, (void*)&raw_obj);
	}

	if(create_shader(&data->cube_shader, vertex_shader, fragment_shader) != 0)
	{
		fprintf(stderr, "failed to create shader for cube\n");
		return 1;
	}

	vec3 cam_pos = { 0.0f, 3.5f, 3.0 };
	object_set_position(&scene->camera.object, cam_pos);
	glm_vec3_zero(&scene->camera.look_at);

	glm_vec3_one(data->ambient_light.color);
	data->ambient_light.intensity = 0.1f;

	glm_vec3_one(data->point_light.color);
	data->point_light.intensity = 1.0f;

	glfwSetMouseButtonCallback(window->window, mouse_button_callback);
	glfwSetCursorPosCallback(window->window, mouse_moved_callback);

	return 0;
}
