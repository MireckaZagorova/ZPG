// Shader.h
#pragma once

#include <GL/glew.h>
#include <string>
#include <iostream>
#include <glm/glm.hpp>


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

    void SetMatrix4(const char* name, const glm::mat4& matrix);

private:
    GLuint programID;

    // Pomocná funkce pro kontrolu chyb kompilace
    void checkCompileErrors(GLuint shader, const std::string& type);
};
