#ifndef CONTEXT_H
#define CONTEXT_H

typedef struct VertexArrayObject VertexArrayObject;

int ctx_init();

void ctx_viewport(int x, int y, int w, int h);

void ctx_clear_screen(float r, float g, float b, float a);
void ctx_draw_elements(VertexArrayObject* vao);

#endif // CONTEXT_H