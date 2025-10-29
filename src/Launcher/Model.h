// Model.h
#pragma once
#include <GL/glew.h>
#include "VertexArrayObject.h"

class Model
{
public:
    Model(VertexArrayObject* vao, int vertexCount) : vao(vao), vertexCount(vertexCount) {};
    void Draw() const;
private:
    VertexArrayObject* vao = nullptr;
    int vertexCount;
};
