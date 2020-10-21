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

using namespace std;

//GLOBALS
const string APP_TITLE = "MinimalApp";
const GLint WIDTH = 1920, HEIGHT = 1080;
GLfloat currentFrame = 0.0f;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// Set it to true to create a full screen application. 
bool gFullScreen = false;

//FUNCTIONS
int INIT_GLFW()
{
	if (!glfwInit())
	{
		cerr << "GLFW Initialization failed" << endl;
		return -1;
	}

	return 0;
}

void INIT_GLEW()
{
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		throw "Error in GLEW INIT!";
	}
}

void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
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

GLFWwindow* INIT_WINDOW(const int WIDTH, const int HEIGHT, int &screenW, int &screenH)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = NULL;

	if (gFullScreen)
	{
		GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* pVmode = glfwGetVideoMode(pMonitor);
		if (pVmode != NULL)
		{
			window = glfwCreateWindow(pVmode->width, pVmode->height, APP_TITLE.c_str(), pMonitor, NULL);
		}
	}
	else
	{
		window = glfwCreateWindow(WIDTH, HEIGHT, APP_TITLE.c_str(), nullptr, nullptr);
	}

	if (window == nullptr)
	{
		glfwTerminate();
		throw("Error in creating window!");
	}

	glfwGetFramebufferSize(window, &screenW, &screenH);

	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, glfw_onKey);

	glViewport(0, 0, screenW, screenH);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return window;
}

int main()
{
	int screenW = 0, screenH = 0;

	//INIT GLFW
	INIT_GLFW();

	//INIT GLFW WINDOW
	GLFWwindow *window = INIT_WINDOW(WIDTH, HEIGHT, screenW, screenH);

	//INIT CONTEXT FOR OPENGL
	INIT_GLEW();

	//BACKFACE CULLING AND CC
	glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CCW); // GL_CCW for counter clock-wise

	 //MAIN LOOP
	while (!glfwWindowShouldClose(window))
	{
		showFPS(window);

		glfwPollEvents();

		//Clear window
		glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Swap buffers
		glfwSwapBuffers(window);
	}

	//TERMINATE GLFW
	glfwTerminate();

	return 0;
}
