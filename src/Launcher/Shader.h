// Shader.h
#pragma once

#include <GL/glew.h>
#include <string>
#include <iostream>
#include <glm/glm.hpp>


class Shader {
public:
    // Konstruktor vytvo�� a zkompiluje shader program
    Shader(const char* vertexSrc, const char* fragmentSrc);

    // Destruktor sma�e shader program
    ~Shader();

    // Aktivuje shader program pro vykreslov�n�
    void use() const;

    // Vr�t� ID shader programu (pokud pot�ebuje� pro debug apod.)
    GLuint getProgramID() const;

    void SetMatrix4(const char* name, const glm::mat4& matrix);

private:
    GLuint programID;

    // Pomocn� funkce pro kontrolu chyb kompilace
    void checkCompileErrors(GLuint shader, const std::string& type);
};
