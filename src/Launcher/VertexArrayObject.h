// VertexArrayObject.h
#pragma once
#include <GL/glew.h>

class VertexArrayObject
{
private:
	GLuint VAO_id;
public:
	VertexArrayObject();
	void Bind();
	void UnBind();
	void AddAttribute(GLuint index, GLint size, GLsizei stride, const void* offset);
};