#include "Application.h"
#include "Shader.h"
#include <iostream>
#include "sphere.h"

// pøidáno pro transformaci
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;



// **************************************************** SHADERY

const char* getVertexShaderPurple() {
    return R"(
        #version 330
        layout(location=0) in vec3 vp;
        void main() {
            gl_Position = vec4(vp, 1.0);
        }
    )";
}
// vykreslení barev podle pozice
const char* getVertexShaderPos() {
    return R"(
        #version 330
        layout(location=0) in vec3 vp;
        out vec3 pos;
        void main() {
            gl_Position = vec4(vp, 1.0);
            pos = vp;
        }
    )";
}

// vykreslení smooth
const char* getVertexShaderNormalColor() {
    return R"(
        #version 330 core
        layout(location=0) in vec3 vp;
        layout(location=1) in vec3 normal;
        out vec3 Normal;
        void main() {
            gl_Position = vec4(vp, 1.0);
            Normal = normal;
        }
    )";
}

//vykreslení flat
const char* getVertexShaderFlat() {
    return R"(
        #version 330 core
        layout(location=0) in vec3 vp;
        layout(location=1) in vec3 normal;
        flat out vec3 Normal;   // <--- flat výstup
        void main() {
            gl_Position = vec4(vp, 1.0);
            Normal = normal;
        }
    )";
}

const char* vertex_shader_trans() {
    return R"(
        #version 330 core
        layout(location=0) in vec3 vp;
        layout(location=1) in vec3 normal;
        uniform mat4 modelMatrix;
        out vec3 Normal;
        void main() {
            gl_Position = modelMatrix * vec4(vp, 1.0);
            Normal = normal;
        }
    )";
}



// *************************************************** FRAGMENTY

const char* getFragmentShaderPurple() {
    return R"(
        #version 330
        out vec4 fragColor;
        void main() {
            fragColor = vec4(0.5, 0.0, 0.5, 1.0);
        }
    )";
}
const char* getFragmentShaderPos() {
    return R"(
        #version 330
        in vec3 pos;
        out vec4 fragColor;
        void main() {
            fragColor = vec4(abs(pos), 1.0);
        }
    )";
}

// vyhlazené sphere 
const char* getFragmentShaderNormalColor() {                    // Normal -> použijeme normálu jako barvu
    return R"(
        #version 330 core
        in vec3 Normal;
        out vec4 fragColor;
        void main() {
            fragColor = vec4(abs(Normal), 1.0);                 
        }
    )";
}

// nevyhlazené sphere - ètvereèky / trojúhelníky
const char* getFragmentShaderFlat() {                              // FLAT je klíèové
    return R"(
        #version 330 core
        flat in vec3 Normal;
        out vec4 fragColor;
        void main() {
            fragColor = vec4(abs(Normal), 1.0);
        }
    )";
}

const char* getFragmentShaderTrans() {                      // vyhlazené normály 
    return R"(
        #version 330 core
        in vec3 Normal;
        out vec4 fragColor;
        void main() {
            fragColor = vec4(abs(Normal), 1.0);
        }
    )";
}



// *************************************************** Konstruktor a Destruktor
Application::Application() : window(nullptr), triangleShader(nullptr), squareShader(nullptr), cv3Shader(nullptr) 
{}     
// NULLPTR - žádný ukazatel (místo null nebo 0) - typovì bezpeèný

Application::~Application() 
{
    if (triangleShader) delete triangleShader;
    if (squareShader) delete squareShader;
    if (cv3Shader) delete cv3Shader;
    if (sphereShader) delete sphereShader;
    if (sphereTransShader) delete sphereTransShader;

    if (modelTriangle) delete modelTriangle;
    if (modelSquare) delete modelSquare;
    if (modelSphere) delete modelSphere;


    if (window) 
    {
        glfwDestroyWindow(window);                    
        glfwTerminate();                             
    }
}



// *************************************************** Inicializace OpenGL / GLFW / GLEW / viewport
void Application::initialization() 
{
    glfwSetErrorCallback([](int error, const char* description) 
    {
        fputs(description, stderr);    
    });


    if (!glfwInit())                               
    {
        cerr << "ERROR: could not start GLFW3" << endl;
        exit(EXIT_FAILURE);
    }

    // Vytvoøení okna 
    window = glfwCreateWindow(800, 600, "Mireccino dilo", NULL, NULL);
    if (!window) 
    {
        glfwTerminate();                            
        exit(EXIT_FAILURE);                         
    }

    glfwMakeContextCurrent(window);              
                                                   
    glfwSwapInterval(1);                           

    glewExperimental = GL_TRUE;                     
    if (glewInit() != GLEW_OK)                     
    {
        cerr << "Failed to initialize GLEW" << endl;
        exit(EXIT_FAILURE);
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);            
}

// *************************************************** Vytvoøení shaderù
void Application::createShaders() 
{
    triangleShader = new Shader(getVertexShaderPurple(), getFragmentShaderPurple());
    squareShader = new Shader(getVertexShaderPos(), getFragmentShaderPos());
    cv3Shader = new Shader(getVertexShaderPos(), getFragmentShaderPos());
    sphereShader = new Shader(getVertexShaderNormalColor(), getFragmentShaderNormalColor());
    sphereTransShader = new Shader(vertex_shader_trans(), getFragmentShaderTrans());

}

// *************************************************** Vytvoøení modelù / VAO / VBO
void Application::createModels() 
{
    // ----------- Trojúhelník
    float points[] = {
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
       -0.5f, -0.5f, 0.0f
    };

    vao_triangle = new VertexArrayObject();                 
    vao_triangle->Bind();

    modelTriangle = new Model(vao_triangle, 3);
    modelSquare = new Model(vao_square, 6);

    vbo_triangle = new VertexBufferObject();
    vbo_triangle->Bind();
    vbo_triangle->SetData(points, sizeof(points));
    vao_triangle->AddAttribute(0, 3, 3 * sizeof(float), (void*)0);

    glBindVertexArray(0);

    // ----------- Ètverec
    float points2[] = {
        0.9f, 0.9f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.9f, 0.6f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.6f, 0.6f, 0.0f, 1.0f, 0.0f, 0.0f,

        0.9f, 0.9f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.6f, 0.9f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.6f, 0.6f, 0.0f, 1.0f, 0.0f, 0.0f
    };

    vao_square = new VertexArrayObject();
    vao_square->Bind();

    vbo_square = new VertexBufferObject();
    vbo_square->Bind();
    vbo_square->SetData(points2, sizeof(points2));
    vao_square->AddAttribute(0, 3, 6 * sizeof(float), (void*)0);
    vao_square->AddAttribute(1, 3, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    modelSquare = new Model(vao_square, 6);
    glBindVertexArray(0);



    // ------------ Nìjaký nový tvar, ještì nevím jaký - otoèená trojùhelník s pravoúhlým rohem vlevo nahoøe

    const float a[] = {
     -.5f, -.5f, .5f,  0, 0, 1,
     -.5f, .5f, .5f,  0, 0, 1,
       .5f, .5f, .5f,  0, 0, 1 };

    vao_cv3 = new VertexArrayObject();
    vao_cv3->Bind();
    vbo_cv3 = new VertexBufferObject();
    vbo_cv3->Bind();
    vbo_cv3->SetData(a, sizeof(a));
    vao_cv3->AddAttribute(0, 3, 6 * sizeof(float), (GLvoid*)0);
    vao_cv3->AddAttribute(1, 3, 6 * sizeof(float), (GLvoid*)(3 * sizeof(float)));

    // ------------ Sphere
    vao_sphere = new VertexArrayObject();
    vao_sphere->Bind();
    vbo_sphere = new VertexBufferObject();
    vbo_sphere->Bind();
    vbo_sphere->SetData(sphere, sizeof(sphere));
    vao_sphere->AddAttribute(0, 3, 6 * sizeof(float), (void*)0);
    vao_sphere->AddAttribute(1, 3, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    modelSphere = new Model(vao_sphere, 2880);
}




// *************************************************** Hlavní smyèka vykreslování
void Application::run() 
{
    while (!glfwWindowShouldClose(window)) 
    {

        glEnable(GL_DEPTH_TEST);             

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Cvièení 2 
        //cviceni2();

        //Cvièení 3
        //cv3_triangle();                       // trojúhelník 
        //cv3_sphere();
        cv3_trans();

        glfwSwapBuffers(window);                
        glfwPollEvents();
    }
}


// ****************************** Info o verzích
void Application::info() 
{
    cout << "OpenGL Version: " << glGetString(GL_VERSION) << endl;
    cout << "Using GLEW " << glewGetString(GLEW_VERSION) << endl;
    cout << "Vendor " << glGetString(GL_VENDOR) << endl;
    cout << "Renderer " << glGetString(GL_RENDERER) << endl;
    cout << "GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

    int major, minor, rev;
    glfwGetVersion(&major, &minor, &rev);
    cout << "Using GLFW " << major << "." << minor << "." << rev << endl;
}



void Application::cviceni2() 
{
    triangleShader->use();
    modelTriangle->Draw();

    squareShader->use();
    modelSquare->Draw();
}

void Application::cv3_triangle() 
{
    cv3Shader->use();
    modelTriangle->Draw();

}

void Application::cv3_sphere()
{
    sphereShader->use();
    modelSphere->Draw();
}

void Application::cv3_trans()
{
    sphereTransShader->use();

    float timeValue = glfwGetTime();
    float angle = timeValue;
    float myView = -2.0f;     

   
    glm::mat4 M = glm::mat4(1.0f);                                      
    M = glm::translate(M, glm::vec3(0.0f, 0.0f, 0.0f));                 
    M = glm::rotate(M, angle, glm::vec3(0.0f, 1.0f, 0.0f));           
    M = glm::rotate(M, angle * 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));   
    M = glm::scale(M, glm::vec3(0.5f));                                

    sphereTransShader->SetMatrix4("modelMatrix", M);
    modelSphere->Draw();
}