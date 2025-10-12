#include "Shader.h"

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

Shader::Shader(const char* vertexSrc, const char* fragmentSrc) {
    // Vytvo�en� vertex shaderu
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSrc, NULL);
    glCompileShader(vertexShader);
    checkCompileErrors(vertexShader, "VERTEX");

    // Vytvo�en� fragment shaderu
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSrc, NULL);
    glCompileShader(fragmentShader);
    checkCompileErrors(fragmentShader, "FRAGMENT");

    // Vytvo�en� shader programu
    programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);
    checkCompileErrors(programID, "PROGRAM");

    // Shadery u� nepot�ebujeme, m��eme je smazat
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    glDeleteProgram(programID);
}

void Shader::use() const {
    glUseProgram(programID);
}

GLuint Shader::getProgramID() const {
    return programID;
}

void Shader::checkCompileErrors(GLuint shader, const string& type) {
    GLint success;
    GLchar infoLog[1024];

    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            cerr << "Shader Compilation Error (" << type << "):\n" << infoLog << endl;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            cerr << "Program Linking Error (" << type << "):\n" << infoLog << endl;
        }
    }
}
