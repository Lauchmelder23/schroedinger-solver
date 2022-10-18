#include "buffer.h"

#include <assert.h>
#include <glad/glad.h>

void create_vao(VertexArrayObject* vao)
{
	assert(vao);
	glGenVertexArrays(1, &vao->vao);
}

void destroy_vao(VertexArrayObject vao)
{
	glDeleteBuffers(1, &vao.ebo);
	glDeleteBuffers(1, &vao.vbo);

	glDeleteVertexArrays(1, &vao.vao);
}

void attach_vertex_buffer(VertexArrayObject* vao, const float* data, size_t size)
{
	assert(vao);
	glBindVertexArray(vao->vao);

	glGenBuffers(1, &vao->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vao->vbo);

	glBufferData(GL_ARRAY_BUFFER, size, (const void*)data, GL_STATIC_DRAW);
}

void attach_element_buffer(VertexArrayObject* vao, const unsigned* data, size_t size)
{
	assert(vao);
	glBindVertexArray(vao->vao);

	glGenBuffers(1, &vao->ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao->ebo);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, (const void*)data, GL_STATIC_DRAW);

	vao->elements = size / sizeof(unsigned);
}

void set_vertex_layout(VertexArrayObject* vao, VertexAttribute* attributes, size_t count)
{
	assert(vao);
	assert(attributes);

	glBindVertexArray(vao->vao);

	size_t stride = 0;
	for (size_t i = 0; i < count; i++) {
		stride += attributes[i].size * attributes[i].count;
	}

	size_t offset = 0;
	for (size_t i = 0; i < count; i++) {
		glVertexAttribPointer(i, attributes[i].count, attributes[i].type, GL_FALSE, stride, (const void*)(offset));
		glEnableVertexAttribArray(i);

		offset += attributes[i].size * attributes[i].count;
	}
}

void bind_vao(VertexArrayObject* vao)
{
	glBindVertexArray(vao->vao);
}

void render_vao(VertexArrayObject* vao)
{
	glDrawArrays(GL_TRIANGLES, 0, vao->elements);
}
