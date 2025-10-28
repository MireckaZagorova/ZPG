// Application.h
#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include "Shader.h" 
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "Model.h"

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
    void cv3_trans();

private:
    GLFWwindow* window;

    // cvièení 2
    VertexArrayObject* vao_triangle;
    VertexBufferObject* vbo_triangle;
    VertexArrayObject* vao_square;
    VertexBufferObject* vbo_square;

    // Ukazatele na shadery
    Shader* triangleShader;
    Shader* squareShader;

    // cvièení 3
    VertexArrayObject* vao_cv3;
    VertexBufferObject* vbo_cv3;
    Shader* cv3Shader;

    // cvièení 3 - koule 
    VertexArrayObject* vao_sphere;
    VertexBufferObject* vbo_sphere;
    Shader* sphereShader;
    Shader* sphereTransShader;

    Model* modelTriangle;
    Model* modelSquare;
    Model* modelSphere;

};
