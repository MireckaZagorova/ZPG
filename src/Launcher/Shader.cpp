#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
/*
 Shader je malý program , který bìží na grafické kartì (GPU) a urèuje, jak se nìco vykreslí
 V OpenGL jsou dva hlavní:
  - Vertex shader - pracuje s jednotlivými vrcholy objektu (vertexy) - napøíklad kde se vertec nachází v prostoru, jak se posune, rotuje, transformuje
  - Fragment shader - pracuje s pixely (fragmenty) a urèuje jejich barvu, texturu, prùhledost atd.
 
 Pøíklad:
  - Vertex shader dostane souøadnice vrcholu trojúhelníku a øekne: „Tento bod se zobrazí tady“.
  - Fragment shader dostane „pixel“ tohoto trojúhelníku a øekne: „Tento pixel bude fialový“.

 */

Shader::Shader(const char* vertexSrc, const char* fragmentSrc) {            // viz prezentace è. 2 - strana 16
    // Vytvoøení vertex shaderu
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSrc, NULL);
    glCompileShader(vertexShader);                                          // TOTO a 
    checkCompileErrors(vertexShader, "VERTEX");                             // Podle typu pozná, co má zkontrolovat (typy - VERTEX, FRAGMENT nebo PROGRAM
    


    // Vytvoøení fragment shaderu
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSrc, NULL);
    glCompileShader(fragmentShader);                                        // TOTO - komplilace shaderu - pobìhlo bez chyb? 
    checkCompileErrors(fragmentShader, "FRAGMENT");                         // Podle typu pozná, co má zkontrolovat (typy - VERTEX, FRAGMENT nebo PROGRAM
    

    // Vytvoøení shader programu - zde se to "slepí"
    programID = glCreateProgram();                                          // Vytvoøí prázdný program na grafické kartì (krabièka, do které se vkládá)
    //Tímto se XXXX (vertexShader nebo fragmentShader´) vloží do programu
    glAttachShader(programID, vertexShader);                                // vertex je ten, který øíká "kde se kadá bod objekt má vykreslt 
    glAttachShader(programID, fragmentShader);                              // fragment shader - ! øeší barvu, bude mít každý pixel?"
    // kontrola, jestli se program správnì "linknul"7
    glLinkProgram(programID);                                               // spojí - "linkne"  shadery dohromady                             
    checkCompileErrors(programID, "PROGRAM");                               // kontrola pøi linkování - viz dole implementace metody


    // Shadery už nepotøebujeme, mùžeme je smazat
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    glDeleteProgram(programID);
}

void Shader::use() const {                                                  // zkrácena cesta k glUseProgra(programID) - kdykoliv chceme aktivovat shader, staèí zavolat
                                                                            //      shader->use/ú
    glUseProgram(programID);
}

GLuint Shader::getProgramID() const {                                       // vrací èíslo programu
    return programID;
}

//*********************************************** Kontrola vzniku a linknutí shaderu/shader programu
void Shader::checkCompileErrors(GLuint shader, const string& type) {
    GLint success;
    GLchar infoLog[1024];

    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);                 // naète informace o shaderu    GL_COMPILE_STATUS - zda shader prošel kompilací 
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);                // naète chybovou hlášku linkeru do do infoLog
            cerr << "Shader Compilation Error (" << type << "):\n" << infoLog << endl;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);                   // naète informace o shaderu    G_LINK_STATUS - zda se shadery správnì spojily do programu
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            cerr << "Program Linking Error (" << type << "):\n" << infoLog << endl;
        }
    }
}


void Shader::SetMatrix4(const char* name, const glm::mat4& matrix)
{
    GLuint loc = glGetUniformLocation(programID, name);
    if (loc == -1)
        std::cerr << "modelMatrix not found in shader!" << std::endl;
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

