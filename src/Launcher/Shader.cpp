#include "Shader.h"

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

Shader::Shader(const char* vertexSrc, const char* fragmentSrc) {
    // Vytvoøení vertex shaderu
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSrc, NULL);
    glCompileShader(vertexShader);
    checkCompileErrors(vertexShader, "VERTEX");

    // Vytvoøení fragment shaderu
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSrc, NULL);
    glCompileShader(fragmentShader);
    checkCompileErrors(fragmentShader, "FRAGMENT");

    // Vytvoøení shader programu
    programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);
    checkCompileErrors(programID, "PROGRAM");

    // Shadery už nepotøebujeme, mùžeme je smazat
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
