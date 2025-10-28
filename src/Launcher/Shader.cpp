#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
/*
 Shader je mal� program , kter� b�� na grafick� kart� (GPU) a ur�uje, jak se n�co vykresl�
 V OpenGL jsou dva hlavn�:
  - Vertex shader - pracuje s jednotliv�mi vrcholy objektu (vertexy) - nap��klad kde se vertec nach�z� v prostoru, jak se posune, rotuje, transformuje
  - Fragment shader - pracuje s pixely (fragmenty) a ur�uje jejich barvu, texturu, pr�hledost atd.
 
 P��klad:
  - Vertex shader dostane sou�adnice vrcholu troj�heln�ku a �ekne: �Tento bod se zobraz� tady�.
  - Fragment shader dostane �pixel� tohoto troj�heln�ku a �ekne: �Tento pixel bude fialov��.

 */

Shader::Shader(const char* vertexSrc, const char* fragmentSrc) {            // viz prezentace �. 2 - strana 16
    // Vytvo�en� vertex shaderu
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSrc, NULL);
    glCompileShader(vertexShader);                                          // TOTO a 
    checkCompileErrors(vertexShader, "VERTEX");                             // Podle typu pozn�, co m� zkontrolovat (typy - VERTEX, FRAGMENT nebo PROGRAM
    


    // Vytvo�en� fragment shaderu
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSrc, NULL);
    glCompileShader(fragmentShader);                                        // TOTO - komplilace shaderu - pob�hlo bez chyb? 
    checkCompileErrors(fragmentShader, "FRAGMENT");                         // Podle typu pozn�, co m� zkontrolovat (typy - VERTEX, FRAGMENT nebo PROGRAM
    

    // Vytvo�en� shader programu - zde se to "slep�"
    programID = glCreateProgram();                                          // Vytvo�� pr�zdn� program na grafick� kart� (krabi�ka, do kter� se vkl�d�)
    //T�mto se XXXX (vertexShader nebo fragmentShader�) vlo�� do programu
    glAttachShader(programID, vertexShader);                                // vertex je ten, kter� ��k� "kde se kad� bod objekt m� vykreslt 
    glAttachShader(programID, fragmentShader);                              // fragment shader - ! �e�� barvu, bude m�t ka�d� pixel?"
    // kontrola, jestli se program spr�vn� "linknul"7
    glLinkProgram(programID);                                               // spoj� - "linkne"  shadery dohromady                             
    checkCompileErrors(programID, "PROGRAM");                               // kontrola p�i linkov�n� - viz dole implementace metody


    // Shadery u� nepot�ebujeme, m��eme je smazat
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    glDeleteProgram(programID);
}

void Shader::use() const {                                                  // zkr�cena cesta k glUseProgra(programID) - kdykoliv chceme aktivovat shader, sta�� zavolat
                                                                            //      shader->use/�
    glUseProgram(programID);
}

GLuint Shader::getProgramID() const {                                       // vrac� ��slo programu
    return programID;
}

//*********************************************** Kontrola vzniku a linknut� shaderu/shader programu
void Shader::checkCompileErrors(GLuint shader, const string& type) {
    GLint success;
    GLchar infoLog[1024];

    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);                 // na�te informace o shaderu    GL_COMPILE_STATUS - zda shader pro�el kompilac� 
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);                // na�te chybovou hl�ku linkeru do do infoLog
            cerr << "Shader Compilation Error (" << type << "):\n" << infoLog << endl;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);                   // na�te informace o shaderu    G_LINK_STATUS - zda se shadery spr�vn� spojily do programu
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

