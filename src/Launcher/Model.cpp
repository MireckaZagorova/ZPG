#pragma once
#include "Model.h"
#include "VertexArrayObject.h"
#include <GL/glew.h>


using namespace std;


Model::Model(VertexArrayObject* vao, int vertexCount)
    : vao(vao), vertexCount(vertexCount)
{
}

void Model::Draw() const
{
    vao->Bind();
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}
