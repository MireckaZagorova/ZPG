// VertexArrayObject.cpp
#include "VertexArrayObject.h"
#include <GL/glew.h>

VertexArrayObject::VertexArrayObject()
{
	glGenVertexArrays(1, &VAO_id);
}

void VertexArrayObject::Bind() 
{
	glBindVertexArray(VAO_id);
}

void VertexArrayObject::UnBind()
{
	glBindVertexArray(0);
}
void VertexArrayObject::AddAttribute(GLuint index, GLint size, GLsizei stride, const void* offset) 
{
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, offset);
}