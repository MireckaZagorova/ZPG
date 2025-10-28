#include "VertexBufferObject.h"
#include <GL/glew.h>

VertexBufferObject::VertexBufferObject()
{
	glGenBuffers(1, &vbo_id);
}

void VertexBufferObject::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
}
void VertexBufferObject::UnBind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBufferObject::SetData(const void* data, GLsizeiptr size)
{
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

