#ifndef WINDOW_H
#define WINDOW_H

typedef struct GLFWwindow GLFWwindow;

typedef struct Window
{
	GLFWwindow* window;
	void(*on_size_change)(void*, int, int);
	void* user_data;
} Window;

int create_managed_window(Window* window, const char* title, int width, int height);
void destroy_window(Window window);

#endif // WINDOW_H