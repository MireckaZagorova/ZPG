#pragma once
#include "Model.h"
#include "VertexArrayObject.h"
#include <GL/glew.h>


using namespace std;

void Model::Draw() const
{
    if (!vao) return;
    vao->Bind();
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}
