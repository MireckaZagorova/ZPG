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
    void cviceni2();
    void cv3_triangle();
    void cv3_sphere();

private:
    GLFWwindow* window;

    // cvièení 2
    GLuint VAO_triangle, VBO_triangle;
    GLuint VAO_square, VBO_square;
    // Ukazatele na shadery
    Shader* triangleShader;
    Shader* squareShader;

    // cvièení 3
    GLuint VAO_cv3, VBO_cv3;
    Shader* cv3Shader;

    // cvièení 3 - koule 
    GLuint VAO_sphere, VBO_sphere;
    Shader* sphereShader;
};
