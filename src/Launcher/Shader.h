#pragma once

#include <GL/glew.h>
#include <string>
#include <iostream>

class Shader {
public:
    // Konstruktor vytvoøí a zkompiluje shader program
    Shader(const char* vertexSrc, const char* fragmentSrc);

    // Destruktor smaže shader program
    ~Shader();

    // Aktivuje shader program pro vykreslování
    void use() const;

    // Vrátí ID shader programu (pokud potøebuješ pro debug apod.)
    GLuint getProgramID() const;

private:
    GLuint programID;

    // Pomocná funkce pro kontrolu chyb kompilace
    void checkCompileErrors(GLuint shader, const std::string& type);
};
