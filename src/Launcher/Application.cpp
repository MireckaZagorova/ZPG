#include "Application.h"
#include "Shader.h"
#include <iostream>
#include "sphere.h"

// p�id�no pro transformaci
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
// vykreslen� barev podle pozice
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

// vykreslen� smooth
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

//vykreslen� flat
const char* getVertexShaderFlat() {
    return R"(
        #version 330 core
        layout(location=0) in vec3 vp;
        layout(location=1) in vec3 normal;
        flat out vec3 Normal;   // <--- flat v�stup
        void main() {
            gl_Position = vec4(vp, 1.0);
            Normal = normal;
        }
    )";
}

// Zde p�vodn� vzhled p�ed zabalen�m do funkc� !!! 
/*
const char* vertex_shader_trans =
"#version 330 core\n"
"layout(location=0) in vec3 vp;\n"                          // pozice vrcholu
"layout(location=1) in vec3 normal;\n"                      // norm�la
"uniform mat4 modelMatrix;\n"                               // uniformn� matice
"out vec3 Normal;\n"                                        // vyhlazen� norm�ly pos�l�me do fragment shader
"void main() {\n"
"    gl_Position = modelMatrix * vec4(vp, 1.0);\n"          // transformace vrcholu  !!! PO�AD� JE D�LE�IT� !!!
"    Normal = normal;\n"                                    // zachov�n� norm�ly
"}";

*/

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

// vyhlazen� sphere 
const char* getFragmentShaderNormalColor() {                    // Normal -> pou�ijeme norm�lu jako barvu
    return R"(
        #version 330 core
        in vec3 Normal;
        out vec4 fragColor;
        void main() {
            fragColor = vec4(abs(Normal), 1.0);                 
        }
    )";
}

// nevyhlazen� sphere - �tvere�ky / troj�heln�ky
const char* getFragmentShaderFlat() {                              // FLAT je kl��ov�
    return R"(
        #version 330 core
        flat in vec3 Normal;
        out vec4 fragColor;
        void main() {
            fragColor = vec4(abs(Normal), 1.0);
        }
    )";
}

const char* getFragmentShaderTrans() {                      // vyhlazen� norm�ly 
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
// NULLPTR - ��dn� ukazatel (m�sto null nebo 0) - typov� bezpe�n�

Application::~Application() 
{
    if (triangleShader) delete triangleShader;
    if (squareShader) delete squareShader;
    if (cv3Shader) delete cv3Shader;
    if (sphereShader) delete sphereShader;
    if (sphereTransShader) delete sphereTransShader;

    if (window) 
    {
        glfwDestroyWindow(window);                   // nejd��ve se zni�� okno 
        glfwTerminate();                             // ukon�� se cel� knihovna -> uvoln�n� v�ech zdroj�
    }
}



// *************************************************** Inicializace OpenGL / GLFW / GLEW / viewport
void Application::initialization() 
{
    // Nastaven� callbacku pro chyby GLFW - ��k�me "kdykoliv nastavne chyba, zavolej tuhle funkci"
    glfwSetErrorCallback([](int error, const char* description) 
    {
        fputs(description, stderr);                 // Vyp�e chybovou zpr�vu do standardn�ho chybov�ho v�stupu (stderr)
    });

    // Inicializace GLFW
    /*
        GLFW - usnad�uje pr�ci s okny, vstupy a kontextem OpenGL
    */
    if (!glfwInit())                                // glfwInit - spou�t� knihovnu)
    {
        cerr << "ERROR: could not start GLFW3" << endl;
        exit(EXIT_FAILURE);
    }

    // Vytvo�en� okna 
    window = glfwCreateWindow(800, 600, "Mireccino dilo", NULL, NULL);
    if (!window) 
    {
        glfwTerminate();                            // ukon�� GLFW a uvoln� v�echny zdroje, kter� vytvo�ilo
        exit(EXIT_FAILURE);                         // okam�it� ukon�� program s informac�, �e nastala chyba
    }

    glfwMakeContextCurrent(window);                 // Vytvo�� kontext = prost�ed�, kde se dr�� v�echna nastaven� (shadery, textury, buffer objekty,...)
                                                    // ��k�, �e pr�v� toto okno (window) bude m�t aktivn� OpenGL kontext - bez toho by se nedalo vykreslovat, proto�e by nev�d�lo kam :-) 
    glfwSwapInterval(1);                            // N�jak� V-Sync synchronizace - zajist� hladk� vykreslov�n� synchronizovan� s monitorem

    // Start GLEW
    // GLEW je knihovna, kter� umo��uje pou��vat modeln� OpenGL funkce
    glewExperimental = GL_TRUE;                     // Povoluje tyto funkce
    if (glewInit() != GLEW_OK)                      // Kontrola
    {
        cerr << "Failed to initialize GLEW" << endl;
        exit(EXIT_FAILURE);
    }

    // Vykreslovac� oblast
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);// Zjist�skute�n� pixelov� rozm�r okna
    glViewport(0, 0, width, height);                // nastav� oblast, kam se kresl� (obvykle cel� okno)
}

// *************************************************** Vytvo�en� shader�
void Application::createShaders() 
{
    triangleShader = new Shader(getVertexShaderPurple(), getFragmentShaderPurple());
    squareShader = new Shader(getVertexShaderPos(), getFragmentShaderPos());
    cv3Shader = new Shader(getVertexShaderPos(), getFragmentShaderPos());
    sphereShader = new Shader(getVertexShaderNormalColor(), getFragmentShaderFlat());
    //sphereShader = new Shader(getVertexShaderFlat(), getFragmentShaderFlat());
    sphereTransShader = new Shader(vertex_shader_trans(), getFragmentShaderTrans());

}

// *************************************************** Vytvo�en� model� / VAO / VBO
void Application::createModels() 
{
    // ----------- Troj�heln�k
    float points[] = {
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
       -0.5f, -0.5f, 0.0f
    };

    glGenVertexArrays(1, &VAO_triangle);
    glGenBuffers(1, &VBO_triangle);

    glBindVertexArray(VAO_triangle);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_triangle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(0);

    // ----------- �tverec
    float points2[] = {
        0.9f, 0.9f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.9f, 0.6f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.6f, 0.6f, 0.0f, 1.0f, 0.0f, 0.0f,

        0.9f, 0.9f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.6f, 0.9f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.6f, 0.6f, 0.0f, 1.0f, 0.0f, 0.0f
    };

    glGenVertexArrays(1, &VAO_square);
    glGenBuffers(1, &VBO_square);

    glBindVertexArray(VAO_square);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_square);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points2), points2, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindVertexArray(0);

    // ------------ N�jak� nov� tvar, je�t� nev�m jak� - oto�en� troj�heln�k s pravo�hl�m rohem vlevo naho�e

    const float a[] = {
     -.5f, -.5f, .5f,  0, 0, 1,
     -.5f, .5f, .5f,  0, 0, 1,
       .5f, .5f, .5f,  0, 0, 1 };

    // p�idejte dal�� atribut 
    //GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO_cv3);
    glGenBuffers(1, &VBO_cv3);

    glBindVertexArray(VAO_cv3);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_cv3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(a), a, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)(3 * sizeof(float)));

    // ------------ Sphere

    glGenVertexArrays(1, &VAO_sphere);
    glGenBuffers(1, &VBO_sphere);

    glBindVertexArray(VAO_sphere);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_sphere);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sphere), sphere, GL_STATIC_DRAW);

    // pozice
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

    // norm�la
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindVertexArray(0);

}




// *************************************************** Hlavn� smy�ka vykreslov�n�
void Application::run() 
{
    while (!glfwWindowShouldClose(window)) 
    {

        glEnable(GL_DEPTH_TEST);              // zapne "vyhlazen�" koule - nejsou troj�heln��ky, ale vyhlazen� 

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Cvi�en� 2 
        //cviceni2();

        //Cvi�en� 3
        //cv3_triangle();                       // troj�heln�k 
        //cv3_sphere();
        cv3_trans();

        glfwSwapBuffers(window);                // prohod� ("swapne") obrazov� buffery -> ��k� n�co jako "V�echno co jsem te� nakreslil, uka� na obrazovku"
        glfwPollEvents();
    }
}


// ****************************** Info o verz�ch
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
    // Troj�heln�k
    triangleShader->use();
    glBindVertexArray(VAO_triangle);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // �tverec
    squareShader->use();
    glBindVertexArray(VAO_square);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Application::cv3_triangle() 
{
    cv3Shader->use();
    glBindVertexArray(VAO_cv3);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

void Application::cv3_sphere()
{
    //glDisable(GL_DEPTH_TEST);                                 // d�l� �tvere�ky 
    //glEnable(GL_DEPTH_TEST);                                    // d�l� troj�heln��ky
    sphereShader->use();
    glBindVertexArray(VAO_sphere);
    glDrawArrays(GL_TRIANGLES, 0, 2880);
    glBindVertexArray(0);

    // !!!!!! sphere m� velikost sphere[17280] -> jeden vrchol pot�ebuje 6  float� (3x sou�adnice, 3x barva) -> 17280/6 = 2880 
}

void Application::cv3_trans()
{
    sphereTransShader->use();

    float timeValue = glfwGetTime();
    float angle = timeValue;  // rotuje podle �asu
    float myView = -2.0f;     // posun dop�edu/dozadu

    // V�po�et transforma�n� matice (rotace, posun, m���tko)
    glm::mat4 M = glm::mat4(1.0f);                                      // jednotkov� matice
    M = glm::translate(M, glm::vec3(0.0f, 0.0f, 0.0f));                 // posun od kamery
    //M = glm::translate(M, glm::vec3(sin(timeValue), 0.0f, -2.0f));
    M = glm::rotate(M, angle, glm::vec3(0.0f, 1.0f, 0.0f));             // oto�en� kolem osy Y
    M = glm::rotate(M, angle * 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));      // m�rn� naklon�n�
    M = glm::scale(M, glm::vec3(0.5f));                                 // velikost

    // Po�li matici do shaderu
    GLuint modelLoc = glGetUniformLocation(sphereTransShader->getProgramID(), "modelMatrix");
    if (modelLoc == -1)
        std::cerr << "modelMatrix not found in shader!" << std::endl;
    // pos�l� matici do shaderu
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(M));

    // Vykreslen� koule
    glBindVertexArray(VAO_sphere);
    glDrawArrays(GL_TRIANGLES, 0, 2880);
    glBindVertexArray(0);
}