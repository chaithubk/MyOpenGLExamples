//STANDARD
#include<iostream>
#include<sstream>
#include<stdlib.h>
#include<time.h>

//GLEW
#define GLEW_STATIC
#include<GL\glew.h>

//GLFW
#include<GLFW\glfw3.h>

//OPENGL MATH GLM
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>

#include "ShaderProgram.h"
#include "Texture2D.h"

using namespace std;

//GLOBALS
const char* APP_TITLE = "Hello Shader";
const GLint gWindowWidth = 1920;
const GLint gWindowHeight = 1080;
GLFWwindow* gWindow = NULL;
bool gWireFrame = false;
const std::string texture1Filename = "aeroplane.png";
const std::string texture2Filename = "crate.jpg";

void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void showFPS(GLFWwindow* window);
bool initOpenGL();

int main()
{
	if (!initOpenGL())
	{
		cerr << "GLFW initialization failed" << endl;
		return -1;
	}

	GLfloat vertices[] = {
		// position					// texture coords
		-0.5f,	 0.5f, 0.0f,		0.0f, 1.0f,		// top left
		 0.5f,   0.5f, 0.0f,		1.0f, 1.0f,		// top right
	     0.5f,  -0.5f, 0.0f,		1.0f, 0.0f,		// bottom right
		-0.5f,  -0.5f, 0.0f,		0.0f, 0.0f		// bottom left	
	};

	GLuint indices[] = {
		0, 1, 2,		// triangle 0
		0, 2, 3			// triangle 1
	};

	// Vertex Buffer Object	
	GLuint vbo;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Vertex Array Object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Format the vertex shader needs to understand
	// Bind vao and vbo before calling the below methods
	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(0);

	// texture coords
	// STRIDE for UV coords is 20 - 5 * size of float
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Index Buffer Object
	// Called as Element Array Buffer in OpenGL 
	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	ShaderProgram shaderProgram;
	shaderProgram.loadShaders("basic.vert", "basic.frag");

	Texture2D texture1;
	texture1.loadTexture(texture1Filename, true);

	Texture2D texture2;
	texture2.loadTexture(texture2Filename, true);

	// Main loop
	while (!glfwWindowShouldClose(gWindow))
	{
		showFPS(gWindow);

		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT);

		texture1.bind(0);
		texture2.bind(1);

		glUniform1i(glGetUniformLocation(shaderProgram.getProgram(), "myTexture1"), 0);
		glUniform1i(glGetUniformLocation(shaderProgram.getProgram(), "myTexture2"), 1);

		shaderProgram.use();

		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(gWindow);
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);

	//TERMINATE GLFW
	glfwTerminate();

	return 0;
}

//FUNCTIONS
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	// Wireframe mode
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		gWireFrame = !gWireFrame;
		if (gWireFrame) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}
}

void showFPS(GLFWwindow* window)
{
	static double previousSeconds = 0.0;
	static int frameCount = 0;
	double elpasedSeconds;
	double currentSeconds = glfwGetTime(); //

	elpasedSeconds = currentSeconds - previousSeconds;

	// limit text update 4 times per second 
	if (elpasedSeconds > 0.25)
	{
		previousSeconds = currentSeconds;
		double fps = (double)frameCount / elpasedSeconds;
		double msPerFrame = 1000.0 / fps;

		std::ostringstream outs;
		outs.precision(3);
		outs << std::fixed
			<< APP_TITLE << "   "
			<< "FPS: " << fps << "   "
			<< "Frame Time: " << msPerFrame << "(ms)";

		glfwSetWindowTitle(window, outs.str().c_str());

		frameCount = 0;
	}

	frameCount++;
}

bool initOpenGL()
{
	if (!glfwInit())
	{
		cerr << "GLFW Initialization failed" << endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	gWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, APP_TITLE, nullptr, nullptr);

	if (gWindow == nullptr)
	{
		glfwTerminate();
		throw("Error in creating window!");
		return false;
	}

	glfwMakeContextCurrent(gWindow);

	glfwSetKeyCallback(gWindow, glfw_onKey);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		throw "Error in GLEW INIT!";
		return false;
	}

	glClearColor(0.2f, 0.3f, 0.2f, 1.0f);

	return true;
}
