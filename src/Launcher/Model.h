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

    // Inicializace modelu: VBO, VAO, pøípadnì EBO
    void initialize(const std::vector<float>& vertices, int vertexSize);

    // Vykreslení modelu
    void draw();

private:
    GLuint VBO;
    GLuint VAO;

    GLsizei vertexCount;  // poèet vrcholù
    int vertexSize;       // velikost jednoho vrcholu v float
};*/
