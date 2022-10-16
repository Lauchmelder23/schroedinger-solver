#ifndef CONTEXT_H
#define CONTEXT_H

int ctx_init();

void ctx_viewport(int x, int y, int w, int h);

void ctx_clear_screen(float r, float g, float b, float a);

#endif // CONTEXT_H