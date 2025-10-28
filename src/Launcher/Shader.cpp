#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

Shader::Shader(const char* vertexSrc, const char* fragmentSrc) {           
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSrc, NULL);
    glCompileShader(vertexShader);                                          
    checkCompileErrors(vertexShader, "VERTEX");                             
    
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSrc, NULL);
    glCompileShader(fragmentShader);                                    
    checkCompileErrors(fragmentShader, "FRAGMENT");          
    
    programID = glCreateProgram();            
    glAttachShader(programID, vertexShader);             
    glAttachShader(programID, fragmentShader);         
    glLinkProgram(programID);                                          
    checkCompileErrors(programID, "PROGRAM");       

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

//*********************************************** Kontrola vzniku a linknutí shaderu/shader programu
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


void Shader::SetMatrix4(const char* name, const glm::mat4& matrix)
{
    GLuint loc = glGetUniformLocation(programID, name);
    if (loc == -1)
        std::cerr << "modelMatrix not found in shader!" << std::endl;
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

