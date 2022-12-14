#include "test_scene.h"

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include <GLFW/glfw3.h>
#include <cglm/affine.h>

#include "objects/arrow.h"
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
"uniform vec3 object_color;"
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
"	vec3 result = (ambient + diffuse + specular) * object_color;"
"	FragColor = vec4(result, 1.0);"
"}";

typedef struct SceneData
{
	Shader default_shader;
	DynamicArray default_objects;

	AmbientLight ambient_light;
	PointLight point_light;

	double time;
} SceneData;

static void set_lighting_uniforms(Shader* shader, Scene* scene)
{
	SceneData* data = scene->child;

	set_uniform_vec3(shader, "ambient_color", data->ambient_light.color);
	set_uniform_float(shader, "ambient_intens", data->ambient_light.intensity);

	set_uniform_vec3(shader, "point_pos", data->point_light.position);
	set_uniform_vec3(shader, "point_col", data->point_light.color);
	set_uniform_float(shader, "point_intens", data->point_light.intensity);

	set_uniform_vec3(shader, "cam_pos", scene->camera.object.position);
}

static void init_spinning_cubes_data(SceneData* scene)
{
	assert(scene);

	scene->default_objects = create_dynamic_array(Object*);
	scene->time = 0;
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	
}

static void mouse_moved_callback(GLFWwindow* window, double xpos, double ypos)
{
	
}

static void on_update(Scene* scene, double frametime)
{
	SceneData* data = scene->child;

	for (int i = 0; i < data->default_objects.size; i++)
	{
		Object* obj = *(Object**)dynamic_array_get(&data->default_objects, i);
		update_object(obj);
	}

	vec3 cam_pos = { 50.0f * sin(2.0f * data->time), 10.0f * cos(data->time), 50.0f * cos(2.0f * data->time)};

	object_set_position(&scene->camera.object, cam_pos);
	data->time += frametime;

	glm_vec3_copy(scene->camera.object.position, data->point_light.position);
}

static void on_render(Scene* scene)
{
	SceneData* data = scene->child;

	bind_shader(&data->default_shader);
	set_lighting_uniforms(&data->default_shader, scene);
	scene_set_camera_uniforms(scene, &data->default_shader);

	for (int i = 0; i < data->default_objects.size; i++)
	{
		shader_add_object(&data->default_shader, *(Object**)dynamic_array_get(&data->default_objects, i));
	}

	shader_render(&data->default_shader);
}

int create_test_scene(Window* window, Scene* scene)
{
	assert(scene);

	create_scene(scene);
	scene->on_update = on_update;
	scene->on_render = on_render;
	scene->child = malloc(sizeof(SceneData));
	init_spinning_cubes_data(scene->child);

	SceneData* data = scene->child;

	Arrow* arrow = (Arrow*)malloc(sizeof(Arrow));
	create_arrow(arrow);
	dynamic_array_push(&data->default_objects, (void*)&arrow);

	if(create_shader(&data->default_shader, vertex_shader, fragment_shader) != 0)
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
