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
GLint gWindowWidth = 1024;
GLint gWindowHeight = 760;
GLFWwindow* gWindow = NULL;
bool gWireFrame = false;
const std::string texture1Filename = "woodcrate_diffuse.jpg";

void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void glfw_onFrameBufferSize(GLFWwindow* window, int width, int height);
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
		
		// front face
		-1.0f,	 1.0f, 1.0f,		0.0f, 1.0f,		
		 1.0f,  -1.0f, 1.0f,		1.0f, 0.0f,		
	     1.0f,   1.0f, 1.0f,		1.0f, 1.0f,		
		-1.0f,   1.0f, 1.0f,		0.0f, 1.0f,
		-1.0f,  -1.0f, 1.0f,		0.0f, 0.0f,
		 1.0f,  -1.0f, 1.0f,		1.0f, 0.0f,

		 // back face
		 -1.0f,	  1.0f, -1.0f,		0.0f, 1.0f,
		  1.0f,  -1.0f, -1.0f,		1.0f, 0.0f,
		  1.0f,   1.0f, -1.0f,		1.0f, 1.0f,
		 -1.0f,   1.0f, -1.0f,		0.0f, 1.0f,
		 -1.0f,  -1.0f, -1.0f,		0.0f, 0.0f,
		  1.0f,  -1.0f, -1.0f,		1.0f, 0.0f,

		  // left face
		  -1.0f,   1.0f, -1.0f,		0.0f, 1.0f,
		  -1.0f,  -1.0f,  1.0f,		1.0f, 0.0f,
		  -1.0f,   1.0f,  1.0f,		1.0f, 1.0f,
		  -1.0f,   1.0f, -1.0f,		0.0f, 1.0f,
		  -1.0f,  -1.0f, -1.0f,		0.0f, 0.0f,
		  -1.0f,  -1.0f,  1.0f,		1.0f, 0.0f,

		   // right face
		    1.0f,	1.0f,  1.0f,	0.0f, 1.0f,
			1.0f,  -1.0f, -1.0f,	1.0f, 0.0f,
			1.0f,   1.0f, -1.0f,	1.0f, 1.0f,
		    1.0f,   1.0f,  1.0f,	0.0f, 1.0f,
		    1.0f,  -1.0f,  1.0f,	0.0f, 0.0f,
			1.0f,  -1.0f, -1.0f,	1.0f, 0.0f,

			// top face
			-1.0f,	 1.0f, -1.0f,	0.0f, 1.0f,
			 1.0f,   1.0f,  1.0f,	1.0f, 0.0f,
			 1.0f,   1.0f, -1.0f,	1.0f, 1.0f,
			-1.0f,   1.0f, -1.0f,	0.0f, 1.0f,
			-1.0f,   1.0f,  1.0f,	0.0f, 0.0f,
			 1.0f,   1.0f,  1.0f,	1.0f, 0.0f,

			 // bottom face
			 -1.0f,	 -1.0f,  1.0f,		0.0f, 1.0f,
			  1.0f,  -1.0f, -1.0f,		1.0f, 0.0f,
			  1.0f,  -1.0f,  1.0f,		1.0f, 1.0f,
			 -1.0f,  -1.0f,  1.0f,		0.0f, 1.0f,
			 -1.0f,  -1.0f, -1.0f,		0.0f, 0.0f,
			  1.0f,  -1.0f, -1.0f,		1.0f, 0.0f,
	};

	glm::vec3 cubePos = glm::vec3(0.0f, 0.0f, -5.0f);

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

	ShaderProgram shaderProgram;
	shaderProgram.loadShaders("basic.vert", "basic.frag");

	Texture2D texture1;
	texture1.loadTexture(texture1Filename, true);

	float cubeAngle = 0.0f;
	double lastTime = glfwGetTime();

	// Main loop
	while (!glfwWindowShouldClose(gWindow))
	{
		showFPS(gWindow);

		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;

		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		texture1.bind(0);

		cubeAngle += (float)(deltaTime * 50.0f);
		if (cubeAngle >= 360.0)
			cubeAngle = 0.0f;

		glm::mat4 model, view, projection;

		model = glm::translate(model, cubePos) * glm::rotate(model, glm::radians(cubeAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::vec3 camPos(0.0f, 0.0f, 0.0f);
		glm::vec3 targetPos(0.0f, 0.0f, -1.0f);
		glm::vec3 up(0.0f, 1.0f, 0.0f);

		view = glm::lookAt(camPos, targetPos, up);
		projection = glm::perspective(glm::radians(45.0f), (float)gWindowWidth / (float)gWindowHeight, 0.1f, 100.0f);

		shaderProgram.use();

		shaderProgram.setUniform("model", model);
		shaderProgram.setUniform("view", view);
		shaderProgram.setUniform("projection", projection);

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glfwSwapBuffers(gWindow);

		lastTime = currentTime;
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);

	//TERMINATE GLFW
	glfwTerminate();

	return 0;
}

void glfw_onFrameBufferSize(GLFWwindow* window, int width, int height)
{
	gWindowWidth = width;
	gWindowHeight = height;
	glViewport(0, 0, gWindowWidth, gWindowHeight);
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
	glfwSetFramebufferSizeCallback(gWindow, glfw_onFrameBufferSize);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		throw "Error in GLEW INIT!";
		return false;
	}

	glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
	glViewport(0, 0, gWindowWidth, gWindowHeight);
	glEnable(GL_DEPTH_TEST);

	return true;
}
