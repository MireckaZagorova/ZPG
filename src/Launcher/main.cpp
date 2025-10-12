#include "Application.h"


int main()
{
    // Vytvoření instance aplikace
    Application* app = new Application();

    // Inicializace OpenGL, GLFW, GLEW, viewport
    app->initialization();

    // Vytvoření shaderů
    app->createShaders();

    // Vytvoření modelů / primitiv
    app->createModels();

    // Spuštění hlavní renderovací smyčky
    app->run();

    // Ukončení aplikace
    delete app;
    return 0;
}