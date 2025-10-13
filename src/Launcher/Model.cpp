#include "Model.h"

using namespace std;

// Model je objekt v programu, který drží informace o tvaru, barvách, texturních souøadnicích, a také VBO/VAO
//    Obsahuje:
//  - Vrcholy(verticles)
//  - Barvy nebo textury (volitelné)
//  - Shader program, který se použije k vykreslení tohoto modelu.
// Model je "jednotka"´, kterou mùžeme vykreslit vícekrát a rùznì transformovat (posouvat, otáèet, mìnit velikost)
// Pøíklad:
//  - 1. model = trojúhelník
//  - 2. model = ètverec
//  -> Každý model má vlastní data a mùže mít i jiný shader (napø. fialový trojúhelník, ètverec - barvy podle pozice)

// JAK SPOLU SOUVISÍ MODEL A SHADER
//  - Model OBSAHUJE DATA a metody k vykreslení objektu
//  - Shader URÈUJE, JAK SE tato data vizuálnì ZOBRAZÍ

//  "... použij tento shader a vykresli tento model..."

//VBO - pamì na GPU, kde jsou uožené vrcholy
//VAO - definuje JAK OpenGL ÈTE VBO = který vertex ný hají atributy


Model::Model()
    : VBO(0), VAO(0), vertexCount(0), vertexSize(0)
    // VBO - promìnná, která drží ID vertex bufferu -> 0 = zatím neexistuje
    // VAO - promìnná, která drží ID vertex array objektu -> 0 = zatím neexistuje
    // vertexCount = poèet vrcholu (napø 3 flaoty pro pozici), také 0
    // vertexSize = veklikost jednoho vrcholu (napø 3 floaty pro pozici, také 0
{
}

Model::~Model()
{
    if (VBO != 0) glDeleteBuffers(1, &VBO);
    if (VAO != 0) glDeleteVertexArrays(1, &VAO);
}


// vertices = seznam èísel (floats), které reprezentují všechny vrcholy modelu, napø. troúhelník: [x1, y1, z1, x2, y2, z2, x3, y3, z3]
// vertexSize = poèet èísel (floats) na jeden vrchol, napø. trojúhelník: 3, pokud máme jen pozici x,y,z

void Model::initialize(const vector<float>& vertices, int vertexSize)
{
    this->vertexSize = vertexSize;
    vertexCount = static_cast<GLsizei>(vertices.size() / vertexSize);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);                                                                                // Vytvoøí Vertex Buffer Object (VBO) -> uložení vrcholù do graf. pamìti
                                                                                                          // 1= jeden buffer, &VBO - uložíme ID bufferu do èlenské promìnné objektu
    // Aktivujeme VBI ori typ GL_ARRAY_BUFFER => všechny operace na GL_ARRAY_BUFFER se vztahují k tomuto VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);                                                                   // GL_ARRAY_BUFFER = typ bufferu pro vertex data (pozice, barvy, normály,..)
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);      // nahrajeme data do GPU  
    // vertices.size() * sizeof(float)  - kolik bajtù dat chceme poslat 
    // vertices.data()                  - ukazatal na data z vektoru vertices
    // GL_STATIS_DRAW                   - informuje OpenGL, že data NEBUDOU ÈASTO MÌNÌNA a budou použita hlavnì pro kreslení

    // VAO                              = popisuje jak èíst data z bufferu
    // VBO                              = samotná data vrcholù uložená v GPU
    // glGenBuffers a glBindBuffer      = vytvoøení a aktivace bufferu
    // glBufferData                     = naplnìní bufferu daty 



    // ******** ZDE SE OPENGL UÈÍ, JAK MÁ ÈÍST MÁ DATA z VBO, když bude kreslit model 

    // pøedpokládáme 3D pozici + pøípadnì další atributy
    glEnableVertexAttribArray(0);           // aktivuje atribut è. 0 - shader mùže èíst data z toho vstupu, standardnì to bývá pozice vrcholu (vec3 position ve vertex shaderu)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), (void*)0);

    // 0                                - èíslo atributu (musí odpovídat tomu, co oèekává shader - layout (location = 0) ve vertex shaderu)
    // 3                                - kolik hodnot tvoøí jeden atribut - tady 3, tedy x,y,z souøadnice
    // GL_FLOAT                         - typ dat v bufferu
    // GL_FALSE                         - nemáme normalizovat hodnoty, ale použít je tak, jak jsou 
    // vertexSize * sizeof(float)       - velikost jednoho vrcholu v bajtech - tzv. stride
    // (void)0*                         - offset - zaèínáme hned od zaèátku bifferu



    // pokud by byly další atributy na jeden vrchol (barvy, normály, texCoords)
    if (vertexSize > 3) {
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, vertexSize - 3, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), (void*)(3 * sizeof(float)));
        // atribut è. 1
        // vertexSize - 3               = poèet zbývajících hodnot po pozici 
        // (void*) (3*sizeof(float))    - data zaèínají až po prvních tøech floatech (tedy po x,y,z)
    }

    glBindVertexArray(0);               // odpojit, uklidit 
}

void Model::draw()
{
    glBindVertexArray(VAO);             // "teï budu kreslit tento model, takže použij tyhle nastavné vrcholy a atributy"
    glDrawArrays(GL_TRIANGLES, 0, vertexCount); // samotné vykreslení
    glBindVertexArray(0);               // odpojí, uklidí 
}
