#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h>

typedef struct VertexArrayObject
{
	int vao;

	int vbo;
	int ebo;

	size_t elements;
} VertexArrayObject;

typedef struct VertexAttribute
{
	int type;
	int count;
	size_t size;
} VertexAttribute;

void create_vao(VertexArrayObject* vao);
void destroy_vao(VertexArrayObject vao);

void attach_vertex_buffer(VertexArrayObject* vao, const float* data, size_t size);
void attach_element_buffer(VertexArrayObject* vao, const unsigned* data, size_t size);

void set_vertex_layout(VertexArrayObject* vao, VertexAttribute* attributes, size_t count);

void bind_vao(VertexArrayObject* vao);
void render_vao(VertexArrayObject* vao);

#endif // BUFFER_H