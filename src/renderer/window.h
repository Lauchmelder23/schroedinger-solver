#ifndef WINDOW_H
#define WINDOW_H

typedef struct GLFWwindow GLFWwindow;

GLFWwindow* create_managed_window(const char* title, int width, int height);

#endif // WINDOW_H