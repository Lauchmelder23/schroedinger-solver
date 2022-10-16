#ifndef WINDOW_H
#define WINDOW_H

typedef struct GLFWwindow GLFWwindow;

GLFWwindow* create_managed_window(const char* title, int width, int height);
void destroy_window(GLFWwindow* window);

#endif // WINDOW_H