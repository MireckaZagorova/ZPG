// VertexBufferObject.h
#pragma once
#include <GL/glew.h>
class VertexBufferObject
{
private: 
	GLuint vbo_id;
public: 
	VertexBufferObject();
	void Bind();
	void UnBind();
	void SetData(const void* data, GLsizeiptr size);


};

