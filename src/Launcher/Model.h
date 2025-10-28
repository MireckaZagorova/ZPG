// Model.h
#pragma once
#include <GL/glew.h>
#include "VertexArrayObject.h"

class Model
{
public:
    Model(VertexArrayObject* vao, int vertexCount);
    void Draw() const;
private:
    VertexArrayObject* vao;
    int vertexCount;
};
