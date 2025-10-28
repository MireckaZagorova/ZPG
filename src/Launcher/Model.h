// Model.h
#pragma once
#include <GL/glew.h>
#include "VertexArrayObject.h"
//#include <vector>

class Model
{
public:
    Model(VertexArrayObject* vao, int vertexCount);
    void Draw() const;
private:
    VertexArrayObject* vao;
    int vertexCount;
};


/*
class Model {
public:
    Model();
    ~Model();

    // Inicializace modelu: VBO, VAO, p��padn� EBO
    void initialize(const std::vector<float>& vertices, int vertexSize);

    // Vykreslen� modelu
    void draw();

private:
    GLuint VBO;
    GLuint VAO;

    GLsizei vertexCount;  // po�et vrchol�
    int vertexSize;       // velikost jednoho vrcholu v float
};*/
