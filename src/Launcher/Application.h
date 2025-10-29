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
    void RenderBasicShapes();
    void RenderTriangle();
    void RenderSphere();
    void RenderRotatingSphere();

private:
    GLFWwindow* window;

    float lastFrameTime = 0.0f;
    float deltaTime = 0.0f;

    // Ukazatele na shadery
    Shader* triangleShader = nullptr;
    Shader* squareShader = nullptr;
	Shader* greenShader = nullptr;

    // Modely
    Model* modelTriangle = nullptr;
    Model* modelSquare = nullptr;
    Model* modelCv3 = nullptr;
    Model* modelSphere = nullptr;
    Model* modelMonkey = nullptr;
    Model* modelTree = nullptr;
    Model* modelBushes = nullptr;

    // scény
    class Scene* baseScene = nullptr;
    class Scene* treesScene = nullptr;
    class Scene* mainScene = nullptr;

    // cvièení 2
    VertexArrayObject* vao_triangle = nullptr;
    VertexBufferObject* vbo_triangle = nullptr;
    VertexArrayObject* vao_square = nullptr;
    VertexBufferObject* vbo_square = nullptr;

    // cvièení 3
    VertexArrayObject* vao_cv3 = nullptr;
    VertexBufferObject* vbo_cv3 = nullptr;
    Shader* cv3Shader = nullptr;

    // cvièení 3 - koule 
    VertexArrayObject* vao_sphere = nullptr;
    VertexBufferObject* vbo_sphere = nullptr;
    Shader* sphereShader = nullptr;
    Shader* sphereTransShader = nullptr;

    // cvièení 3 - monkey
    VertexArrayObject* vao_monkey = nullptr;
    VertexBufferObject* vbo_monkey = nullptr;

    // cvièení 3 - tree
	VertexArrayObject* vao_tree = nullptr;
	VertexBufferObject* vbo_tree = nullptr;

    // cvièení 3 - bushes
	VertexArrayObject* vao_bushes = nullptr;
	VertexBufferObject* vbo_bushes = nullptr;

    

};
