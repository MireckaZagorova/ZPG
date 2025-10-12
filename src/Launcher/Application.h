// Application.h
#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include "Shader.h" 

class Application {
public:
    Application();
    ~Application();

    void initialization();
    void createShaders();
    void createModels();
    void run();
    void info();

private:
    GLFWwindow* window;

    GLuint VAO_triangle, VBO_triangle;
    GLuint VAO_square, VBO_square;

    // Ukazatele na shadery
    Shader* triangleShader;
    Shader* squareShader;
};
