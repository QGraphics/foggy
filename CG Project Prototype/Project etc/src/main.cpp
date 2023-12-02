//-----------------------------------------------------------------------------
#include <iostream>
#include <sstream>

#define GLEW_STATIC

#include "GL/glew.h"    // Important - this header must come before glfw3 header
#include "GLFW/glfw3.h"

//textrue rendering
#include "rendering/ShaderProgram.h"
#include "rendering/Texture.h"
#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include "rendering/Camera.h"
#include "rendering/Mesh.h"

// Global Variables
const char *APP_TITLE = "Computer Graphics";
const int gWindowWidth = 900;
const int gWindowHeight = 720;
GLFWwindow *gWindow = NULL;
bool gWireframe = false;

//Experimenting with transformations and animations
bool transDirection = true;
float offset = 0.0f;
float maxOffset = 0.7f;
float increment = 0.01f;

//rotation
float curAngle = 0.0f;

//scaling
bool sizeDirection = true;
float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

//textures
//const std::string floorImage = "res/images/cubeTest.jpg";

//setting up the camera
FPSCamera fpsCamera(glm::vec3(0.0f, 5.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f));
const double ZOOM_SENSITIVITY = -3.0f;
const float MOVE_SPEED = 5.0f; //units per second
const float MOUSE_SENSETIVITY = 0.1f;

// Function prototypes
void glfw_onMouseScroll(GLFWwindow *window, double deltaX, double deltaY);

void update(double elapsedTime);

void glfw_onKey(GLFWwindow *window, int key, int scancode, int action, int mode);

void glfw_onFramebufferSize(GLFWwindow *window, int width, int height);

void showFPS(GLFWwindow *window);

bool initOpenGL();

//-----------------------------------------------------------------------------
// Main Application Entry Point
//-----------------------------------------------------------------------------

int main() {
	if (!initOpenGL()) {
		// An error occured
		std::cerr << "GLFW initialization failed" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	ShaderProgram CubeMapShader;
	CubeMapShader.loadShaders("res/shaders/cubemap.vert", "res/shaders/cubemap.frag");
	ShaderProgram skyboxShader;
	skyboxShader.loadShaders("res/shaders/skybox.vert", "res/shaders/skybox.frag");

	float cubeVertices[] = {
			// positions          // normals
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
			0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
			0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
			0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
			-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
			0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
			0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
			-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

			-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
			-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
			-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

			0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
			0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

			-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
			0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
			0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
			0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
			-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
			-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
	};
	float skyboxVertices[] = {
			// positions
			-1.0f, 1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,

			-1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f,

			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f,

			-1.0f, 1.0f, -1.0f,
			1.0f, 1.0f, -1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, 1.0f
	};

	// Create the Plane
	Mesh plane;
	plane.generatePlane(2, 10.0f);

	// cube VAO
	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
	// skybox VAO
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);

	// load textures
	// -------------
	vector<std::string> faces
	{
		"res/textures/skybox/right.jpg",
		"res/textures/skybox/left.jpg",
		"res/textures/skybox/top.jpg",
		"res/textures/skybox/bottom.jpg",
		"res/textures/skybox/back.jpg",
		"res/textures/skybox/front.jpg"
	};
	
	Texture cubeTexture;
	unsigned int cubemapTexture = cubeTexture.loadCubemap(faces);

	// shader configuration
	// --------------------
	CubeMapShader.use();
	glUniform1i(glGetUniformLocation(CubeMapShader.getProgram(), "skybox"), 0);

	skyboxShader.use();
	glUniform1i(glGetUniformLocation(skyboxShader.getProgram(), "skybox"), 0);

	//call shaders
	ShaderProgram shaderProgram;
	shaderProgram.loadShaders("res/shaders/transform.vert", "res/shaders/transform.frag");

	//****** Texture ******
	Texture texture1;
	texture1.loadTexture("res/images/Pigeon.jpg", true);

	double lastTime = glfwGetTime();

	// Rendering loop
	while (!glfwWindowShouldClose(gWindow)) {
		showFPS(gWindow);

		double currentTime = glfwGetTime();
		double deltatime = currentTime - lastTime;

		// Poll for and process events
		glfwPollEvents();
		update(deltatime);

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render the quad (two triangles)
		shaderProgram.use();

		//add transformation
		glm::mat4 projection(1.0f), view(1.0f), model(1.0f);

		//Create the view matrix
		view = fpsCamera.getViewMatrix();

		//project matrix
		projection = glm::perspective(glm::radians(fpsCamera.getFOV()), (float) gWindowWidth / (float) gWindowHeight,
									  0.1f, 100.0f);

		//pass the matrices to the shader
		shaderProgram.setUniform("model", model);
		shaderProgram.setUniform("view", view);
		shaderProgram.setUniform("projection", projection);

		//bind texture
		texture1.bind(1);
		plane.draw();
		texture1.unbind(1);

		// draw skybox as last
		glDepthFunc(
				GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		/**/skyboxShader.use();
		view = glm::mat4(glm::mat3(fpsCamera.getViewMatrix())); // remove translation from the view matrix
		skyboxShader.setUniform("view", view);
		skyboxShader.setUniform("projection", projection);
		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		/**/

		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default

		// Swap front and back buffers
		glfwSwapBuffers(gWindow);
		lastTime = currentTime;
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &skyboxVBO);

	glfwTerminate();
	return 0;
}

//-----------------------------------------------------------------------------
// Initialize GLFW and OpenGL
//-----------------------------------------------------------------------------
bool initOpenGL() {
	// Intialize GLFW
	// GLFW is configured.  Must be called before calling any GLFW functions
	if (!glfwInit()) {
		// An error occured
		std::cerr << "GLFW initialization failed" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,
				   GL_TRUE);    // forward compatible with newer versions of OpenGL as they become available but not backward compatible (it will not run on devices that do not support OpenGL 3.3

	// Create an OpenGL 3.3 core, forward compatible context window
	gWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, APP_TITLE, NULL, NULL);
	if (gWindow == NULL) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	// Make the window's context the current one
	glfwMakeContextCurrent(gWindow);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return false;
	}

	// Set the required callback functions
	glfwSetKeyCallback(gWindow, glfw_onKey);
	glfwSetFramebufferSizeCallback(gWindow, glfw_onFramebufferSize);
	glfwSetScrollCallback(gWindow, glfw_onMouseScroll);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Define the viewport dimensions
	glViewport(0, 0, gWindowWidth, gWindowHeight);

	// Depth testing
	glEnable(GL_DEPTH_TEST);

	return true;
}

//-----------------------------------------------------------------------------
// Is called whenever a key is pressed/released via GLFW
//-----------------------------------------------------------------------------
void glfw_onKey(GLFWwindow *window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_F && action == GLFW_PRESS) {
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
void glfw_onFramebufferSize(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

void glfw_onMouseScroll(GLFWwindow *window, double deltaX, double deltaY) {
	double fov = fpsCamera.getFOV() + deltaY * ZOOM_SENSITIVITY;
	fov = glm::clamp(fov, 1.0, 120.0);
	fpsCamera.setFOV((float) fov);
}

void update(double elapsedTime) {
	//camer orientation
	double mouseX, mouseY;

	//get current mouse cursor position delta
	glfwGetCursorPos(gWindow, &mouseX, &mouseY);

	if (glfwGetKey(gWindow, GLFW_KEY_R) == GLFW_PRESS) {
		//rotate the camera using the dlta in mouse differences
		fpsCamera.rotate(
			float(gWindowWidth / 2.0f - mouseX) * MOUSE_SENSETIVITY,
			float(gWindowHeight / 2.0f - mouseY) * MOUSE_SENSETIVITY
		);

		glfwSetCursorPos(gWindow, gWindowWidth / 2.0, gWindowHeight / 2.0);
	}

	if (glfwGetKey(gWindow, GLFW_KEY_G) == GLFW_PRESS) {
		//rotate the camera using the dlta in mouse differences
		fpsCamera.rotate(
			float(gWindowWidth / 2.0f - mouseX) * MOUSE_SENSETIVITY,
			float(gWindowHeight / 2.0f - mouseY) * MOUSE_SENSETIVITY
		);

		glfwSetCursorPos(gWindow, gWindowWidth / 2.0, gWindowHeight / 2.0);
	}

	glm::vec3
		flatUp(0.0f, 1.0f, 0.0f),
		flatLook(fpsCamera.getLook().x, 0.0, fpsCamera.getLook().z),
		flatRight(fpsCamera.getRight().x, 0.0, fpsCamera.getRight().z)
	;

	//camera move forwad backword
	if (glfwGetKey(gWindow, GLFW_KEY_W) == GLFW_PRESS)
		fpsCamera.move((float) MOVE_SPEED * (float) elapsedTime * flatLook);
	else if (glfwGetKey(gWindow, GLFW_KEY_S) == GLFW_PRESS)
		fpsCamera.move((float) MOVE_SPEED * (float) elapsedTime * -flatLook);

	//left right
	if (glfwGetKey(gWindow, GLFW_KEY_A) == GLFW_PRESS)
		fpsCamera.move((float) MOVE_SPEED * (float) elapsedTime * -flatRight);
	else if (glfwGetKey(gWindow, GLFW_KEY_D) == GLFW_PRESS)
		fpsCamera.move((float) MOVE_SPEED * (float) elapsedTime * flatRight);

	//up down
	if (glfwGetKey(gWindow, GLFW_KEY_Z) == GLFW_PRESS)
		fpsCamera.move((float) MOVE_SPEED * (float) elapsedTime * flatUp);
	else if (glfwGetKey(gWindow, GLFW_KEY_X) == GLFW_PRESS)
		fpsCamera.move((float) MOVE_SPEED * (float) elapsedTime * -flatUp);
}

//-----------------------------------------------------------------------------
// Code computes the average frames per second, and also the average time it takes
// to render one frame.  These stats are appended to the window caption bar.
//-----------------------------------------------------------------------------
void showFPS(GLFWwindow *window) {
	static double previousSeconds = 0.0;
	static int frameCount = 0;
	double elapsedSeconds;
	double currentSeconds = glfwGetTime(); // returns number of seconds since GLFW started, as double float

	elapsedSeconds = currentSeconds - previousSeconds;

	// Limit text updates to 4 times per second
	if (elapsedSeconds > 0.25) {
		previousSeconds = currentSeconds;
		double fps = (double) frameCount / elapsedSeconds;
		double msPerFrame = 1000.0 / fps;

		// The C++ way of setting the window title
		std::ostringstream outs;
		outs.precision(3);    // decimal places
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