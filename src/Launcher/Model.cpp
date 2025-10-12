#include "Model.h"

using namespace std;

/*

 Model je objekt v programu, kter� dr�� informace o tvaru, barv�ch, texturn�ch sou�adnic�ch, a tak� VBO/VAO.
 Obsahuje:
  - Vrcholy(verticles)
  - Barvy nebo textury (voliteln�)
  - Shader program, kter� se pou�ije k vykreslen� tohoto modelu.
 Model je "jednotka"�, kterou m��eme vykreslit v�cekr�t a r�zn� transformovat (posouvat, ot��et, m�nit velikost)
 P��klad:
  - 1. model = troj�heln�k
  - 2. model = �tverec
  -> Ka�d� model m� vlastn� data a m��e m�t i jin� shader (nap�. fialov� troj�heln�k, �tverec - barvy podle pozice)

 JAK SPOLU SOUVIS� MODEL A SHADER
  - Model OBSAHUJE DATA a metody k vykreslen� objektu
  - Shader UR�UJE, JAK SE tato data vizu�ln� ZOBRAZ�

  "... pou�ij tento shader a vykresli tento model..."

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

    // p�edpokl�d�me 3D pozici + p��padn� dal�� atributy
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), (void*)0);

    // pokud by byly dal�� atributy (barvy, norm�ly, texCoords)
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
