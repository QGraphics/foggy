//-----------------------------------------------------------------------------
// blinnPhongShading.cpp
//
// - Simple Phong Shading Model
// - Then Blinn-Phong Model
// - Change Shaders
// - Add Light Source
// Textures for the Celestial Bodies (minus the Sun) belong to NASA, except for Mars, which belongs to Solar System Scope
//-----------------------------------------------------------------------------
#include <iostream>
#include <sstream>
#define GLEW_STATIC
#include "GL/glew.h"	// Important - this header must come before glfw3 header lol
#include "GLFW/glfw3.h"
#include "GLM//gtc/matrix_transform.hpp"

// My Custom Classes
#include "ShaderProgram.h"
#include "Texture.h"
#include "Camera.h"
#include "Mesh.h"
#include "GameObject.h"
#include "Celestial.h"

// Global Variables
const char* APP_TITLE = "Computer Graphics - Project Prototype";
const int gWindowWidth = 800;
const int gWindowHeight = 800;
GLFWwindow* gWindow = NULL;
bool gWireframe = false;

// Shaders

// Setting the Camera
FPSCamera fpsCam(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f));
const double ZOOM_SENSITIVITY = -1.0;
const float MOVE_SPEED = 5.0f;
const float MOUSE_SENSITIVITY = 0.1f;

// Experimenting with transformation and animation
bool transDirection = true;
float offset = 0.0f;
float maxOffset = 0.7f;
float increment = 0.001f;

//Rotation
float curAngle = 0.0f;

//Scaling
bool sizeDirection = true;
float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

// Some physics
float downwardVelocity = 4.0f;
float jumpForce = 0.0f;
const float JUMP_MAX = 5.0f;
const float GRAVITY = 9.8f;
bool canJump = false;

// Function prototypes
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void glfw_onFramebufferSize(GLFWwindow* window, int width, int height);
void glfw_onMouseScroll(GLFWwindow* window, double deltaX, double deltaY);
void update(double elapsedTime);
void showFPS(GLFWwindow* window);
bool initOpenGL();

//-----------------------------------------------------------------------------
// Main Application Entry Point
//-----------------------------------------------------------------------------
int main()
{
	if (!initOpenGL())
	{
		// An error occured
		std::cerr << "GLFW initialization failed" << std::endl;
		return -1;
	}

	// Load Meshes
	const int modelCount = 5;

	float G = 1.0f; // The Gravitational Constant. The real world value is too small to be of use here

	Celestial celestials[modelCount];

	Celestial earth, sun;
	earth.loadObj("res/models/Earth.obj");
	earth.loadTex("res/models/Earth.jpg");

	sun.loadObj("res/models/bulb.obj");
	sun.loadTex("res/models/Red.png");

	celestials[0].loadObj("res/models/bulb.obj");	// Sun
	celestials[1].loadObj("res/models/Globe.obj");	// Earth
	celestials[2].loadObj("res/models/Globe.obj");	// Moon
	celestials[3].loadObj("res/models/Globe.obj");	// Jupiter
	celestials[4].loadObj("res/models/Globe.obj");	// Mars

	celestials[0].loadTex("res/models/Red.png");	 // Sun
	celestials[1].loadTex("res/models/Earth.jpg");	 // Earth
	celestials[2].loadTex("res/models/Moon.jpg");	 // Moon
	celestials[3].loadTex("res/models/Jupiter.jpg"); // Jupiter
	celestials[4].loadTex("res/models/Mars.jpg");	 // Mars

	// Going to center the Sun ofc
	celestials[0].setPosition(glm::vec3());
	celestials[1].setPosition(glm::vec3(0.0f, 20.0f, 0.0f));
	celestials[2].setPosition(glm::vec3(20.0f, 0.0f, 0.0f));
	celestials[3].setPosition(glm::vec3(0.0f, 0.0f, 50.0f));
	celestials[4].setPosition(glm::vec3(-25.0f, 0.0f, -25.0f));

	cout << (sqrtf(G * 50.0f / 7)) << endl;
	// We need to set the velocities, otherwise they will fall into the Sun
	celestials[1].addVelocity(glm::vec3(0.5f, 0.0f, 0.0f));
	celestials[2].addVelocity(glm::vec3(0.0f, 0.0f, 0.5f));
	celestials[3].addVelocity(glm::vec3(0.3f, 0.0f, 0.0f));
	celestials[4].addVelocity(glm::vec3(0.0f, -0.03f, 0.0f));

	// Logarithmically based on their real life radii in Mm (1000 km)
	int divisior = 1;
	celestials[0].setRadius(log10f(696) / divisior);
	celestials[1].setRadius(log10f(6.37) / divisior);
	celestials[2].setRadius(log10f(1.74) / divisior);
	celestials[3].setRadius(log10f(69.9) / divisior);
	celestials[4].setRadius(log10f(3.39) / divisior);

	// Since we're working with Newtonian physics, these objects need to have masses
	// Of course, we will scale them way down
	celestials[0].setMass(50);
	celestials[1].setMass(2);
	celestials[2].setMass(0.5f);
	celestials[3].setMass(7);
	celestials[4].setMass(1);

	float cubeSize = 2.0f;

	// 2. Set up buffers on the GPU


	// Call shaders
	ShaderProgram shaderProgram;
	shaderProgram.loadShaders("res/shaders/lighting.vert", "res/shaders/lighting.frag");

	// Shader for lights
	ShaderProgram lightProgram;
	lightProgram.loadShaders("res/shaders/bulb.vert", "res/shaders/bulb.frag");

	double oldTime = glfwGetTime();
	float cubeAngle = 0.0f;

	// Rendering loop
	while (!glfwWindowShouldClose(gWindow))
	{
		showFPS(gWindow);

		double currentTime = glfwGetTime();
		double deltaTime = currentTime - oldTime;

		// Poll for and process events
		glfwPollEvents();
		update(deltaTime);

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render the quad (two triangles) 
		shaderProgram.use();
		
		//set uniform for color with some fun
		GLfloat time = (GLfloat)glfwGetTime();
		
		// Add transform code
		glm::mat4
			model,
			projection,
			view
		;

		// Create View Mat
		view = fpsCam.getViewMatrix();

		// Create the Projection Mat
		float
			fovy = 45,
			aspect = (float)gWindowWidth / gWindowHeight,
			zNear = 0.1f,
			zFar = 700
		;

		projection = glm::perspective(glm::radians(fpsCam.getFOV()), aspect, zNear, zFar);

		// Set Projection and View Uniforms
		shaderProgram.setUniform("projection", projection);
		shaderProgram.setUniform("view", view);

		// Update the view position
		glm::vec3 viewPosition;
		viewPosition.x = fpsCam.getPosition().x;
		viewPosition.y = fpsCam.getPosition().y;
		viewPosition.z = fpsCam.getPosition().z;

		glm::vec3
			lightPos(0.0f, 4.0f, 0.0f),
			lightColor(1.0f, 1.0f, 1.0f)
		;

		// For light:
		shaderProgram.setUniform("viewPos", viewPosition);
		shaderProgram.setUniform("lightPos", lightPos);
		shaderProgram.setUniform("lightColor", lightColor);

		/*
			Create a Loop for uhhhhhh
			Calculating gravitational force acting on each object, as well as uhhh
			Calculating new velocities and positions etc
			a = - G * m_other * r_hat / r^2
			r_hat = normalize(position_this - position_other)
		*/
		
		for (int i = 0; i < modelCount; i++)
		{
			for (int j = 0; j < modelCount; j++)
			{
				if (i != j)
				{
					glm::vec3 r_hat = glm::normalize(celestials[i].getPosition() - celestials[j].getPosition());
					float r = glm::distance(celestials[i].getPosition(), celestials[j].getPosition());
					glm::vec3 a = -G * celestials[j].getMass() * r_hat / (powf(r, 2) + 0.1f);

					celestials[i].addVelocity((float)deltaTime * a);
					celestials[i].setPosition(
						(float)deltaTime * celestials[i].getVelocity() + celestials[i].getPosition()
					);
				}
			}
		}

		// Render the scene
		for (int i = 0; i < modelCount; i++)
		{
			model =
				glm::translate(glm::mat4(1.0f), celestials[i].getPosition()) *
				glm::scale(glm::mat4(1.0f), celestials[i].getRadii());

			if (i == 0)
			{
				// Render the Sun (bulb geometry)
				lightProgram.use();

				lightProgram.setUniform("lightColor", lightColor);
				lightProgram.setUniform("model", model);
				lightProgram.setUniform("projection", projection);
				lightProgram.setUniform("view", view);

				celestials[i].render();

				shaderProgram.use();
			}

			shaderProgram.setUniform("model", model);

			celestials[i].render();

			//if (i == 2)	cout << gameObjs[i].detectCollision(&gameObjs[3]);
		}

		glBindVertexArray(0);

		// Swap front and back buffers
		glfwSwapBuffers(gWindow);

		oldTime = currentTime;
	}
	
	glfwTerminate();

	return 0;
}

//-----------------------------------------------------------------------------
// Initialize GLFW and OpenGL
//-----------------------------------------------------------------------------
bool initOpenGL()
{
	// Intialize GLFW 
	// GLFW is configured.  Must be called before calling any GLFW functions
	if (!glfwInit())
	{
		// An error occured
		std::cerr << "GLFW initialization failed" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);	// forward compatible with newer versions of OpenGL as they become available but not backward compatible (it will not run on devices that do not support OpenGL 3.3

	// Create an OpenGL 3.3 core, forward compatible context window
	gWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, APP_TITLE, NULL, NULL);
	if (gWindow == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	// Make the window's context the current one
	glfwMakeContextCurrent(gWindow);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return false;
	}

	// Set the required callback functions
	glfwSetKeyCallback(gWindow, glfw_onKey);
	glfwSetFramebufferSizeCallback(gWindow, glfw_onFramebufferSize);
	glfwSetScrollCallback(gWindow, glfw_onMouseScroll);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	// Define the viewport dimensions
	glViewport(0, 0, gWindowWidth, gWindowHeight);

	// Add depth check
	glEnable(GL_DEPTH_TEST);

	return true;
}

//-----------------------------------------------------------------------------
// Is called whenever a key is pressed/released via GLFW
//-----------------------------------------------------------------------------
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	
	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		gWireframe = !gWireframe;
		if (gWireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

//-----------------------------------------------------------------------------
// Is called when the window is resized
//-----------------------------------------------------------------------------
void glfw_onFramebufferSize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//-----------------------------------------------------------------------------
// Code computes the average frames per second, and also the average time it takes
// to render one frame.  These stats are appended to the window caption bar.
//-----------------------------------------------------------------------------
void showFPS(GLFWwindow* window)
{
	static double previousSeconds = 0.0;
	static int frameCount = 0;
	double elapsedSeconds;
	double currentSeconds = glfwGetTime(); // returns number of seconds since GLFW started, as double float

	elapsedSeconds = currentSeconds - previousSeconds;

	// Limit text updates to 4 times per second
	if (elapsedSeconds > 0.25)
	{
		previousSeconds = currentSeconds;
		double fps = (double)frameCount / elapsedSeconds;
		double msPerFrame = 1000.0 / fps;

		// The C++ way of setting the window title
		std::ostringstream outs;
		outs.precision(3);	// decimal places
		outs << std::fixed
			<< APP_TITLE << "    "
			<< "FPS: " << fps << "    "
			<< "Frame Time: " << msPerFrame << " (ms)";
		glfwSetWindowTitle(window, outs.str().c_str());

		// Reset for next average.
		frameCount = 0;
	}

	frameCount++;
}

void glfw_onMouseScroll(GLFWwindow* window, double deltaX, double deltaY)
{
	glm::vec3 move = fpsCam.getLook();
	move *= deltaY * ZOOM_SENSITIVITY;

	fpsCam.move(-move);
}

void update(double elapsedTime)
{
	//camer orientation
	double mouseX, mouseY;

	//get current mouse cursor position delta
	glfwGetCursorPos(gWindow, &mouseX, &mouseY);

	//rotate the camera using the dlta in mouse differences
	fpsCam.rotate(float(gWindowWidth / 2.0f - mouseX) * MOUSE_SENSITIVITY,
		float(gWindowHeight / 2.0f - mouseY) * MOUSE_SENSITIVITY);

	glfwSetCursorPos(gWindow, gWindowWidth / 2.0, gWindowHeight / 2.0);

	glm::vec3
		flatLook(fpsCam.getLook().x, 0.0f, fpsCam.getLook().z),
		flatRight(fpsCam.getRight().x, 0.0f, fpsCam.getRight().z)
	;

	flatLook = glm::normalize(flatLook);
	flatRight = glm::normalize(flatRight);

	//camera move forwad backword
	if (glfwGetKey(gWindow, GLFW_KEY_W) == GLFW_PRESS)
		fpsCam.move((float)MOVE_SPEED * (float)elapsedTime * flatLook);
	else if (glfwGetKey(gWindow, GLFW_KEY_S) == GLFW_PRESS)
		fpsCam.move((float)MOVE_SPEED * (float)elapsedTime * -flatLook);

	//left right
	if (glfwGetKey(gWindow, GLFW_KEY_D) == GLFW_PRESS)
		fpsCam.move((float)MOVE_SPEED * (float)elapsedTime * flatRight);
	else if (glfwGetKey(gWindow, GLFW_KEY_A) == GLFW_PRESS)
		fpsCam.move((float)MOVE_SPEED * (float)elapsedTime * -flatRight);

	//up down
	if (glfwGetKey(gWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
		fpsCam.move((float)MOVE_SPEED * (float)elapsedTime * glm::vec3(0.0f, 1.0f, 0.0f));
	else if (glfwGetKey(gWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		fpsCam.move((float)MOVE_SPEED * (float)elapsedTime * -glm::vec3(0.0f, 1.0f, 0.0f));
}