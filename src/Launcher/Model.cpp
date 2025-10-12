#include "Model.h"

using namespace std;

/*

 Model je objekt v programu, který drží informace o tvaru, barvách, texturních souøadnicích, a také VBO/VAO.
 Obsahuje:
  - Vrcholy(verticles)
  - Barvy nebo textury (volitelné)
  - Shader program, který se použije k vykreslení tohoto modelu.
 Model je "jednotka"´, kterou mùžeme vykreslit vícekrát a rùznì transformovat (posouvat, otáèet, mìnit velikost)
 Pøíklad:
  - 1. model = trojúhelník
  - 2. model = ètverec
  -> Každý model má vlastní data a mùže mít i jiný shader (napø. fialový trojúhelník, ètverec - barvy podle pozice)

 JAK SPOLU SOUVISÍ MODEL A SHADER
  - Model OBSAHUJE DATA a metody k vykreslení objektu
  - Shader URÈUJE, JAK SE tato data vizuálnì ZOBRAZÍ

  "... použij tento shader a vykresli tento model..."

*/

Model::Model()
    : VBO(0), VAO(0), vertexCount(0), vertexSize(0)
{
}

Model::~Model()
{
    if (VBO != 0) glDeleteBuffers(1, &VBO);
    if (VAO != 0) glDeleteVertexArrays(1, &VAO);
}

void Model::initialize(const vector<float>& vertices, int vertexSize)
{
    this->vertexSize = vertexSize;
    vertexCount = static_cast<GLsizei>(vertices.size() / vertexSize);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // pøedpokládáme 3D pozici + pøípadnì další atributy
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), (void*)0);

    // pokud by byly další atributy (barvy, normály, texCoords)
    if (vertexSize > 3) {
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, vertexSize - 3, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), (void*)(3 * sizeof(float)));
    }

    glBindVertexArray(0); // odpojit
}

void Model::draw()
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glBindVertexArray(0);
}
