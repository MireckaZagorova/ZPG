#include "Application.h"
#include "Shader.h"
#include <iostream>
#include "sphere.h"

using namespace std;



// ****************************** SHADERY

const char* vertex_shader_purple =
"#version 330\n"
"layout(location=0) in vec3 vp;"
"void main () {"
"     gl_Position = vec4 (vp, 1.0);"
"}";

const char* vertex_shader_pos =
"#version 330\n"
"layout(location=0) in vec3 vp;"
"out vec3 pos;"
"void main () {"
"     gl_Position = vec4 (vp, 1.0);"
"     pos = vp;"
"}";

// ****************************** FRAGMENTY

const char* fragment_shader_purple =
"#version 330\n"
"out vec4 fragColor;"
"void main () {"
"     fragColor = vec4 (0.5, 0.0, 0.5, 1.0);"
"}";

const char* fragment_shader_pos =
"#version 330\n"
"in vec3 pos;"
"out vec4 fragColor;"
"void main () {"
"    fragColor = vec4(abs(pos), 1.0);"
"}";



// ****************************** Konstruktor a Destruktor
Application::Application() : window(nullptr), triangleShader(nullptr), squareShader(nullptr), cv3Shader(nullptr) 
{}     
// NULLPTR - žádný ukazatel (místo null nebo 0) - typovì bezpeèný

Application::~Application() 
{
    if (triangleShader) delete triangleShader;
    if (squareShader) delete squareShader;
    if (cv3Shader) delete cv3Shader;
    if (sphereShader) delete sphereShader;

    if (window) 
    {
        glfwDestroyWindow(window);                   // nejdøíve se zniøí okno 
        glfwTerminate();                             // ukonèí se celá knihovna -> uvolnìní všech zdrojù
    }
}

// ****************************** Inicializace OpenGL / GLFW / GLEW / viewport
void Application::initialization() 
{
    // Nastavení callbacku pro chyby GLFW - øíkáme "kdykoliv nastavne chyba, zavolej tuhle funkci"
    glfwSetErrorCallback([](int error, const char* description) 
    {
        fputs(description, stderr);                 // Vypíše chybovou zprávu do standardního chybového výstupu (stderr)
    });

    // Inicializace GLFW
    /*
        GLFW - usnadòuje práci s okny, vstupy a kontextem OpenGL
    */
    if (!glfwInit())                                // glfwInit - spouští knihovnu)
    {
        cerr << "ERROR: could not start GLFW3" << endl;
        exit(EXIT_FAILURE);
    }

    // Vytvoøení okna 
    window = glfwCreateWindow(800, 600, "Mireccino dilo", NULL, NULL);
    if (!window) 
    {
        glfwTerminate();                            // ukonèí GLFW a uvolní všechny zdroje, které vytvoøilo
        exit(EXIT_FAILURE);                         // okamžitì ukonèí program s informací, že nastala chyba
    }

    glfwMakeContextCurrent(window);                 // Vytvoøí kontext = prostøedí, kde se drží všechna nastavení (shadery, textury, buffer objekty,...)
                                                    // Øíká, že právì toto okno (window) bude mít aktivní OpenGL kontext - bez toho by se nedalo vykreslovat, protože by nevìdìlo kam :-) 
    glfwSwapInterval(1);                            // Nìjaká V-Sync synchronizace - zajistí hladké vykreslování synchronizované s monitorem

    // Start GLEW
    // GLEW je knihovna, která umožòuje používat modelní OpenGL funkce
    glewExperimental = GL_TRUE;                     // Povoluje tyto funkce
    if (glewInit() != GLEW_OK)                      // Kontrola
    {
        cerr << "Failed to initialize GLEW" << endl;
        exit(EXIT_FAILURE);
    }

    // Vykreslovací oblast
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);// Zjistískuteèná pixelový rozmìr okna
    glViewport(0, 0, width, height);                // nastaví oblast, kam se kreslí (obvykle celé okno)
}

// ****************************** Vytvoøení shaderù
void Application::createShaders() 
{
    triangleShader = new Shader(vertex_shader_purple, fragment_shader_purple);
    squareShader = new Shader(vertex_shader_pos, fragment_shader_pos);
    cv3Shader = new Shader(vertex_shader_pos, fragment_shader_pos);
    sphereShader = new Shader(vertex_shader_purple, fragment_shader_purple);

}

// ****************************** Vytvoøení modelù / VAO / VBO
void Application::createModels() 
{
    // ----------- Trojúhelník
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

    // ----------- Ètverec
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

    // ------------ Nìjaký nový tvar, ještì nevím jaký - otoèená trojùhelník s pravoúhlým rohem vlevo nahoøe

    const float a[] = {
     -.5f, -.5f, .5f,  0, 0, 1,
     -.5f, .5f, .5f,  0, 0, 1,
       .5f, .5f, .5f,  0, 0, 1 };

    // pøidejte další atribut 
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

    // normála
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindVertexArray(0);



}

// ****************************** Hlavní smyèka vykreslování
void Application::run() 
{
    while (!glfwWindowShouldClose(window)) 
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Cvièení 2 
        //cviceni2();

        //Cvièení 3
        //cv3_triangle();           // trojúhelník 

        cv3_sphere();
        


        glfwSwapBuffers(window);                // prohodí ("swapne") obrazové buffery -> øíká nìco jako "Všechno co jsem teï nakreslil, ukaž na obrazovku"
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
    // Trojúhelník
    triangleShader->use();
    glBindVertexArray(VAO_triangle);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Ètverec
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
    glDisable(GL_DEPTH_TEST);
    sphereShader->use();
    glBindVertexArray(VAO_sphere);
    glDrawArrays(GL_TRIANGLES, 0, 2880);
    glBindVertexArray(0);

    // !!!!!! sphere má velikost sphere[17280] -> jeden vrchol potøebuje 6  floatù (3x souøadnice, 3x barva) -> 17280/6 = 2880 
}