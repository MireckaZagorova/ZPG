//Include GLEW
#include <GL/glew.h>
//Include GLFW
#include <GLFW/glfw3.h>


//Include the standard C++ headers  
#include <stdlib.h>
#include <stdio.h>
#include <glm/detail/type_vec4.hpp>


struct a {
	float position[4];
	float color[4];
};


// ----------------------------------------------------------------------------------------------------- TVARY

float points[] = {
	0.0f, 0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
   -0.5f, -0.5f, 0.0f
};

// -------------------------- 2 trojuhelniky = ctverec, není nutno přepisovat VAO 
float square[] = {
	0.5f, 0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
   -0.5f, -0.5f, 0.0f,

	0.5f, 0.5f, 0.0f,
   -0.5f, 0.5f, 0.0f,
   -0.5f, -0.5f, 0.0f
};


const a b[] = {
   { { -.5f, -.5f, .5f, 1 }, { 1, 1, 0, 1 } },
   { { -.5f, .5f, .5f, 1 }, { 1, 0, 0, 1 } },
   { { .5f, .5f, .5f, 1 }, { 0, 0, 0, 1 } },
   { { .5f, -.5f, .5f, 1 }, { 0, 1, 0, 1 } },
};

// -------------------------- 2 trojuhelniky = ctverec, je nutno přepsat VAO
float points2[] = {
	0.9f, 0.9f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.9f, 0.6f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.6f, 0.6f, 0.0f, 1.0f, 0.0f, 0.0f,

	0.9f, 0.9f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.6f, 0.9f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.6f, 0.6f, 0.0f, 1.0f, 0.0f, 0.0f
};

const char* vertex_shader_purple =
"#version 330\n"
"layout(location=0) in vec3 vp;"
"void main () {"
"     gl_Position = vec4 (vp, 1.0);"
"}";


const char* fragment_shader_purple =
"#version 330\n"
"out vec4 fragColor;"
"void main () {"
"     fragColor = vec4 (0.5, 0.0, 0.5, 1.0);"				// původní
"}";





const char* vertex_shader_pos =
"#version 330\n"
"layout(location=0) in vec3 vp;"
"out vec3 pos;"												// přidáno
"void main () {"
"     gl_Position = vec4 (vp, 1.0);"
"	  pos = vp; "											// přidáno - pošle pozici dál do fragment shaderu
"}";


const char* fragment_shader_pos =
"#version 330\n"
"in vec3 pos;"												// přidáno
"out vec4 fragColor;"
//"out vec4 spatneJmeno;"
"void main () {"
"    fragColor = vec4(abs(pos), 1.0);"						//vyměněno - barva = pozice 
//"	 fragColor = vec4(1.0, 1.0, 1.0, 1.0); "					// obarví bíle
"}";




static void error_callback(int error, const char* description) { fputs(description, stderr); }








int main(void)
{


	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		exit(EXIT_FAILURE);
	}

	/* //inicializace konkretni verze
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE,
	GLFW_OPENGL_CORE_PROFILE);  //*/

	window = glfwCreateWindow(800, 600, "ZPG", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}



	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();


	// get version info
	printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
	printf("Using GLEW %s\n", glewGetString(GLEW_VERSION));
	printf("Vendor %s\n", glGetString(GL_VENDOR));
	printf("Renderer %s\n", glGetString(GL_RENDERER));
	printf("GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	int major, minor, revision;
	glfwGetVersion(&major, &minor, &revision);
	printf("Using GLFW %i.%i.%i\n", major, minor, revision);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	float ratio = width / (float)height;
	glViewport(0, 0, width, height);






	// ------------------------------------------------------------------------------------------------ VBO, VAO


	// ---------------------------------------------------------------- trojúhelník

	//vertex buffer object (VBO)
	GLuint VBO_triangle = 0;
	glGenBuffers(1, &VBO_triangle); // generate the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO_triangle);

	// ------------------------------------  naplneni VBO
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(b), b, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(points2), points2, GL_STATIC_DRAW);




	//Vertex Array Object (VAO)
	GLuint VAO_triangle = 0;
	glGenVertexArrays(1, &VAO_triangle); //generate the VAO
	glBindVertexArray(VAO_triangle); //bind the VAO
	glEnableVertexAttribArray(0); //enable vertex attributes
	glBindBuffer(GL_ARRAY_BUFFER, VBO_triangle);
	//index, pocet, typ, normalized, velikost jednoho vrcholu, pocatek
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);						// pro points, square
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), NULL);						// pro points2, b


	/* ------------------------ pro body s barvou, je nutno přepsat VAO
	* ------------------------- pro points2, b
	* ------------------------- prozkoumat
	* 

	glEnableVertexAttribArray(0); //enable vertex attributes 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)0);
	glEnableVertexAttribArray(1); //enable vertex attributes 1
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
	*/



	// ---------------------------------------------------------------- čtverec

	GLuint VBO_square = 0;
	glGenBuffers(1, &VBO_square); // generate the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO_square);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points2), points2, GL_STATIC_DRAW);


	GLuint VAO_square = 0;
	glGenVertexArrays(1, &VAO_square); //generate the VAO
	glBindVertexArray(VAO_square); //bind the VAO
	glEnableVertexAttribArray(0); //enable vertex attributes
	glBindBuffer(GL_ARRAY_BUFFER, VBO_square);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), NULL);			
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)0);			


	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));


// ------------------------------------------------------------------------------------------------




	//create and compile shaders
	// ----------------------------------- trojuhelnik
	GLuint vertexShader_triangle = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader_triangle, 1, &vertex_shader_purple, NULL);
	glCompileShader(vertexShader_triangle);

	GLuint fragmentShader_triangle = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader_triangle, 1, &fragment_shader_purple, NULL);
	glCompileShader(fragmentShader_triangle);

	GLuint shaderProgram_triangle = glCreateProgram();
	glAttachShader(shaderProgram_triangle, fragmentShader_triangle);
	glAttachShader(shaderProgram_triangle, vertexShader_triangle);
	glLinkProgram(shaderProgram_triangle);

	// ----------------------------------- čtverec
	GLuint vertexShader_square = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader_square, 1, &vertex_shader_pos, NULL);
	glCompileShader(vertexShader_square);

	GLuint fragmentShader_square = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader_square, 1, &fragment_shader_pos, NULL);
	glCompileShader(fragmentShader_square);

	GLuint shaderProgram_square = glCreateProgram();
	glAttachShader(shaderProgram_square, fragmentShader_square);
	glAttachShader(shaderProgram_square, vertexShader_square);
	glLinkProgram(shaderProgram_square);







	GLint status;
	glGetProgramiv(shaderProgram_triangle, GL_LINK_STATUS, &status);
	
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(shaderProgram_triangle, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(shaderProgram_triangle, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;

	}

	glGetProgramiv(shaderProgram_square, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(shaderProgram_square, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(shaderProgram_square, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;

	}

	// ------------------------------------------------------------------------------------------------  WHILE LOOP			
	 
			
	while (!glfwWindowShouldClose(window)) {
		// clear color and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glUseProgram(shaderProgram_triangle);
		glBindVertexArray(VAO_triangle);	//upraveno
		// ----------------------------------------------------vykreslovani
		// draw triangles
		glDrawArrays(GL_TRIANGLES, 0, 3); //mode,first,count						// pro points

		glUseProgram(shaderProgram_square);
		glBindVertexArray(VAO_square);	// přidáno
		// vykreslí čtverec
		glDrawArrays(GL_TRIANGLES, 0, 6); //mode,first,count						// pro square, points2, b


		// update other events like input handling
		glfwPollEvents();
		// put the stuff we’ve been drawing onto the display
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}
