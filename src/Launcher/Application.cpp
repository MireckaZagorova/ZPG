#include "Application.h"
#include "Shader.h"
#include <iostream>
#include "sphere.h"
#include "suzi_flat.h"
#include "tree.h"
#include "bushes.h"
#include "Scene.h"

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
//u_Time - bude dostávat hodnotu z Cpp (glfwGetTime())
// pùvodnì - float pulse = abs(sin(u_Time)); - jen svícení zelená -> èerná (zmizí)
//           float pulse = 0.4 + abs(sin(u_Time * 2.0)) * 0.6; - nikdy nestmavne do èerné 
const char* getFragmentShaderGreen() {
    return R"(
        #version 330 core
        uniform float u_Time;             
        out vec4 fragColor;
        void main() {
            float pulse = 0.4 + abs(sin(u_Time * 2.0)) * 0.6;
            fragColor = vec4(0.0, pulse, 0.0, 1.0);
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
Application::Application() : window(nullptr)
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

    if (vao_triangle) delete vao_triangle;
    if (vbo_triangle) delete vbo_triangle;
    if (vao_square) delete vao_square;
    if (vbo_square) delete vbo_square;
    if (vao_cv3) delete vao_cv3;
    if (vbo_cv3) delete vbo_cv3;
    if (vao_sphere) delete vao_sphere;
    if (vbo_sphere) delete vbo_sphere;
	if (vao_monkey) delete vao_monkey;
	if (vbo_monkey) delete vbo_monkey;
	if (modelMonkey) delete modelMonkey;
    if (vao_tree) delete vao_tree;
    if (vbo_tree) delete vbo_tree;
    if (modelTree) delete modelTree;
	if (vao_bushes) delete vao_bushes;
	if (vbo_bushes) delete vbo_bushes;
    if (modelBushes) delete modelBushes;

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
    //cv3Shader = new Shader(getVertexShaderPos(), getFragmentShaderPos());
    //sphereShader = new Shader(getVertexShaderNormalColor(), getFragmentShaderNormalColor());
    sphereTransShader = new Shader(vertex_shader_trans(), getFragmentShaderTrans());
    greenShader = new Shader(vertex_shader_trans(), getFragmentShaderGreen());

}

// *************************************************** Vytvoøení modelù / VAO / VBO
/*
// Obecný postup:
//    1) Bind VAO
//    2) Bind VBO
//    3) Naplnit data do VBO
//    4) Nastavit atributy VAO
//    5) Unbind VAO
//    6) Vytvoøit Model z VAO a poètu vrcholù
*/


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

    vbo_triangle = new VertexBufferObject();
    vbo_triangle->Bind();
    vbo_triangle->SetData(points, sizeof(points));
    vao_triangle->AddAttribute(0, 3, 3 * sizeof(float), (void*)0);

    glBindVertexArray(0);
    modelTriangle = new Model(vao_triangle, 3);
    modelSquare = new Model(vao_square, 6);

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

    glBindVertexArray(0);
    modelSquare = new Model(vao_square, 6);


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

    glBindVertexArray(0);
    modelCv3 = new Model(vao_cv3, 3);


    // ------------ Sphere
    vao_sphere = new VertexArrayObject();
    vao_sphere->Bind();
    vbo_sphere = new VertexBufferObject();
    vbo_sphere->Bind();
    vbo_sphere->SetData(sphere, sizeof(sphere));
    vao_sphere->AddAttribute(0, 3, 6 * sizeof(float), (void*)0);
    vao_sphere->AddAttribute(1, 3, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindVertexArray(0);
    modelSphere = new Model(vao_sphere, 2880);

    // ----------  Monkey 
    int suziFlatCount = 17424 / 6;
    vao_monkey = new VertexArrayObject();
    vao_monkey->Bind();
    vbo_monkey = new VertexBufferObject();
    vbo_monkey->Bind();
    vbo_monkey->SetData(suziFlat, sizeof(suziFlat));
    vao_monkey->AddAttribute(0, 3, 6 * sizeof(float), (void*)0);
    vao_monkey->AddAttribute(1, 3, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
    modelMonkey = new Model(vao_monkey, suziFlatCount);

	vao_tree = new VertexArrayObject();
	vao_tree->Bind();
	vbo_tree = new VertexBufferObject();
	vbo_tree->Bind();
	vbo_tree->SetData(tree, sizeof(tree));
	vao_tree->AddAttribute(0, 3, 6 * sizeof(float), (void*)0);
	vao_tree->AddAttribute(1, 3, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);
	modelTree = new Model(vao_tree, 92814);

	vao_bushes = new VertexArrayObject();
	vao_bushes->Bind();
	vbo_bushes = new VertexBufferObject();
	vbo_bushes->Bind();
	vbo_bushes->SetData(bushes, sizeof(bushes));
	vao_bushes->AddAttribute(0, 3, 6 * sizeof(float), (void*)0);
	vao_bushes->AddAttribute(1, 3, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);
	modelBushes = new Model(vao_bushes, 8730);

    // scény
    baseScene = new Scene();
    treesScene = new Scene();
    mainScene = new Scene();

    // baseScene:
    glm::mat4 O1,O2,O3;   // O = object
    O1 = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.0f, -0.5f)); // koule vlevo
    O1 = glm::scale(O1, glm::vec3(0.1f));
    O2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, -0.5f)); // opice vpravo
    O2 = glm::scale(O2, glm::vec3(0.1f));
    O3 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, -0.5f)); // strom dolù
    O3 = glm::scale(O3, glm::vec3(0.1f));

    baseScene->Add(modelSphere, sphereTransShader, O1);
    baseScene->Add(modelMonkey, greenShader, O2);
    baseScene->Add(modelTree, sphereTransShader, O3);

    // treesScene
    glm::mat4 T1, T2, T3;       // T = tree
    T1 = glm::translate(glm::mat4(1.0f), glm::vec3(-0.4f, 0.0f, -0.4f));
    T1 = glm::scale(T1, glm::vec3(0.1f));
    T2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.4f));
    T2 = glm::scale(T2, glm::vec3(0.1f));
    T3 = glm::translate(glm::mat4(1.0f), glm::vec3(0.4f, 0.0f, -0.4f));
    T3 = glm::scale(T3, glm::vec3(0.1f));

    treesScene->Add(modelTree, greenShader, T1);
    treesScene->Add(modelTree, greenShader, T2);
    treesScene->Add(modelTree, greenShader, T3);



}




// *************************************************** Hlavní smyèka vykreslování
void Application::run() 
{
    mainScene = treesScene;             //aktuální scéna
    while (!glfwWindowShouldClose(window)) 
    {
        float currentTime = glfwGetTime();                  // èas teï 
        deltaTime = currentTime - lastFrameTime;            // rozdíl - kolik èasu ubìhlo
        lastFrameTime = currentTime;                        // èas pøed 1 snímkem 
        
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS ||
            glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS)
        {
            mainScene = baseScene;
        }

        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS ||
            glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS)
        {
            mainScene = treesScene;
        }

        glEnable(GL_DEPTH_TEST);             
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Cvièení 2 
        //RenderBasicShapes();

        //Cvièení 3
        //RenderTriangle();                       // trojúhelník 
        //RenderSphere();
        //RenderRotatingSphere();

        float timeValue = glfwGetTime();

        //baseScene->Draw();
        mainScene->Draw();

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



void Application::RenderBasicShapes()
{
    triangleShader->use();
    modelTriangle->Draw();

    squareShader->use();
    modelSquare->Draw();

    cv3Shader->use();
    modelCv3->Draw();
}

void Application::RenderTriangle()
{
    cv3Shader->use();
    modelTriangle->Draw();

}

void Application::RenderSphere()
{
    sphereShader->use();
    modelSphere->Draw();
}

void Application::RenderRotatingSphere()
{
    //sphereTransShader->use();
	greenShader->use();
    
    float timeValue = glfwGetTime();
    float angle = timeValue;

    glm::mat4 M = glm::mat4(1.0f); // koule
    glm::mat4 M2 = glm::mat4(1.0f); // opièka
	glm::mat4 M3 = glm::mat4(1.0f); // strom
	glm::mat4 M4 = glm::mat4(1.0f); // bushes

                                      
    M = glm::translate(M, glm::vec3(0.0f, 0.0f, 0.0f));                 
    M = glm::rotate(M, angle, glm::vec3(0.0f, 1.0f, 0.0f));           
    M = glm::rotate(M, angle * 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));   
    M = glm::scale(M, glm::vec3(0.5f));  

    M2 = glm::translate(M2, glm::vec3(0.0f, 0.0f, 0.0f)); // uprostøed
    M2 = glm::scale(M2, glm::vec3(0.5f)); // a je vidìt
    M2 = glm::rotate(M2, angle, glm::vec3(0.0f, 1.0f, 0.0f));

	M3 = glm::translate(M3, glm::vec3(0.0f, -0.7f, 0.0f)); // vedle
	M3 = glm::scale(M3, glm::vec3(0.2f)); // a je vidìt
	M3 = glm::rotate(M3, angle, glm::vec3(0.0f, 1.0f, 0.0f));

	M4 = glm::translate(M4, glm::vec3(0.0f, 0.0f, 0.0f)); // bushes
	//M4 = glm::scale(M4, glm::vec3(0.5f)); // a je vidìt
	M4 = glm::rotate(M4, angle, glm::vec3(0.0f, 1.0f, 0.0f));


    sphereTransShader->SetMatrix4("modelMatrix", M);
    //modelSphere->Draw();

    sphereTransShader->SetMatrix4("modelMatrix", M2);
    //modelMonkey->Draw();

	sphereTransShader->SetMatrix4("modelMatrix", M3);
	//modelTree->Draw();

	//sphereTransShader->SetMatrix4("modelMatrix", M4);
	greenShader->SetMatrix4("modelMatrix", M4);
	

    //timeValue = glfwGetTime();
    //greenShader->SetFloat("u_Time", timeValue);
    modelBushes->Draw();


}