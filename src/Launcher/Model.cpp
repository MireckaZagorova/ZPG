#include "Model.h"

using namespace std;

// Model je objekt v programu, kter� dr�� informace o tvaru, barv�ch, texturn�ch sou�adnic�ch, a tak� VBO/VAO
//    Obsahuje:
//  - Vrcholy(verticles)
//  - Barvy nebo textury (voliteln�)
//  - Shader program, kter� se pou�ije k vykreslen� tohoto modelu.
// Model je "jednotka"�, kterou m��eme vykreslit v�cekr�t a r�zn� transformovat (posouvat, ot��et, m�nit velikost)
// P��klad:
//  - 1. model = troj�heln�k
//  - 2. model = �tverec
//  -> Ka�d� model m� vlastn� data a m��e m�t i jin� shader (nap�. fialov� troj�heln�k, �tverec - barvy podle pozice)

// JAK SPOLU SOUVIS� MODEL A SHADER
//  - Model OBSAHUJE DATA a metody k vykreslen� objektu
//  - Shader UR�UJE, JAK SE tato data vizu�ln� ZOBRAZ�

//  "... pou�ij tento shader a vykresli tento model..."

//VBO - pam� na GPU, kde jsou uo�en� vrcholy
//VAO - definuje JAK OpenGL �TE VBO = kter� vertex n� haj� atributy


Model::Model()
    : VBO(0), VAO(0), vertexCount(0), vertexSize(0)
    // VBO - prom�nn�, kter� dr�� ID vertex bufferu -> 0 = zat�m neexistuje
    // VAO - prom�nn�, kter� dr�� ID vertex array objektu -> 0 = zat�m neexistuje
    // vertexCount = po�et vrcholu (nap� 3 flaoty pro pozici), tak� 0
    // vertexSize = veklikost jednoho vrcholu (nap� 3 floaty pro pozici, tak� 0
{
}

Model::~Model()
{
    if (VBO != 0) glDeleteBuffers(1, &VBO);
    if (VAO != 0) glDeleteVertexArrays(1, &VAO);
}


// vertices = seznam ��sel (floats), kter� reprezentuj� v�echny vrcholy modelu, nap�. tro�heln�k: [x1, y1, z1, x2, y2, z2, x3, y3, z3]
// vertexSize = po�et ��sel (floats) na jeden vrchol, nap�. troj�heln�k: 3, pokud m�me jen pozici x,y,z

void Model::initialize(const vector<float>& vertices, int vertexSize)
{
    this->vertexSize = vertexSize;
    vertexCount = static_cast<GLsizei>(vertices.size() / vertexSize);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);                                                                                // Vytvo�� Vertex Buffer Object (VBO) -> ulo�en� vrchol� do graf. pam�ti
                                                                                                          // 1= jeden buffer, &VBO - ulo��me ID bufferu do �lensk� prom�nn� objektu
    // Aktivujeme VBI ori typ GL_ARRAY_BUFFER => v�echny operace na GL_ARRAY_BUFFER se vztahuj� k tomuto VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);                                                                   // GL_ARRAY_BUFFER = typ bufferu pro vertex data (pozice, barvy, norm�ly,..)
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);      // nahrajeme data do GPU  
    // vertices.size() * sizeof(float)  - kolik bajt� dat chceme poslat 
    // vertices.data()                  - ukazatal na data z vektoru vertices
    // GL_STATIS_DRAW                   - informuje OpenGL, �e data NEBUDOU �ASTO M�N�NA a budou pou�ita hlavn� pro kreslen�

    // VAO                              = popisuje jak ��st data z bufferu
    // VBO                              = samotn� data vrchol� ulo�en� v GPU
    // glGenBuffers a glBindBuffer      = vytvo�en� a aktivace bufferu
    // glBufferData                     = napln�n� bufferu daty 



    // ******** ZDE SE OPENGL U��, JAK M� ��ST M� DATA z VBO, kdy� bude kreslit model 

    // p�edpokl�d�me 3D pozici + p��padn� dal�� atributy
    glEnableVertexAttribArray(0);           // aktivuje atribut �. 0 - shader m��e ��st data z toho vstupu, standardn� to b�v� pozice vrcholu (vec3 position ve vertex shaderu)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), (void*)0);

    // 0                                - ��slo atributu (mus� odpov�dat tomu, co o�ek�v� shader - layout (location = 0) ve vertex shaderu)
    // 3                                - kolik hodnot tvo�� jeden atribut - tady 3, tedy x,y,z sou�adnice
    // GL_FLOAT                         - typ dat v bufferu
    // GL_FALSE                         - nem�me normalizovat hodnoty, ale pou��t je tak, jak jsou 
    // vertexSize * sizeof(float)       - velikost jednoho vrcholu v bajtech - tzv. stride
    // (void)0*                         - offset - za��n�me hned od za��tku bifferu



    // pokud by byly dal�� atributy na jeden vrchol (barvy, norm�ly, texCoords)
    if (vertexSize > 3) {
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, vertexSize - 3, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), (void*)(3 * sizeof(float)));
        // atribut �. 1
        // vertexSize - 3               = po�et zb�vaj�c�ch hodnot po pozici 
        // (void*) (3*sizeof(float))    - data za��naj� a� po prvn�ch t�ech floatech (tedy po x,y,z)
    }

    glBindVertexArray(0);               // odpojit, uklidit 
}

void Model::draw()
{
    glBindVertexArray(VAO);             // "te� budu kreslit tento model, tak�e pou�ij tyhle nastavn� vrcholy a atributy"
    glDrawArrays(GL_TRIANGLES, 0, vertexCount); // samotn� vykreslen�
    glBindVertexArray(0);               // odpoj�, uklid� 
}
