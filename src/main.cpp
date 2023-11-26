//-----------------------------------------------------------------------------
// HelloTriangle_3.cpp
//
// - Adds index buffer (Element Array Buffer)
//-----------------------------------------------------------------------------
#include <iostream>
#include <sstream>

#define GLEW_STATIC

#include "GL/glew.h"    // Important - this header must come before glfw3 header
#include "GLFW/glfw3.h"
#include "rendering/ShaderProgram.h"
#include "rendering/Texture.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"


// Global Variables
const char *APP_TITLE = "Computer Graphics";
const int gWindowWidth = 800;
const int gWindowHeight = 600;
GLFWwindow *gWindow = NULL;
bool gWireframe = false;

bool transDirection = true;
float offset = 0.0f;
float maxOffset = 0.7f;
float increment = 0.001f;
float tempValues = 1.0f;
float currentAngle = 0.0f;

bool sizeDirection = true;
float currentSize = 0.4f;
float maximumSize = 0.5f;
float minimumSize = 0.1f;

// Shaders

// Function prototypes
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


    glEnable(GL_DEPTH_TEST);




    // Rendering loop
    while (!glfwWindowShouldClose(gWindow)) {
        showFPS(gWindow);

        // Poll for and process events
        glfwPollEvents();

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT);



        glBindVertexArray(0); //unbind


        // Swap front and back buffers
        glfwSwapBuffers(gWindow);
    }

    // Clean up

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

    glClearColor(0.23f, 0.38f, 0.47f, 1.0f);

    // Define the viewport dimensions
    glViewport(0, 0, gWindowWidth, gWindowHeight);

    return true;
}

//-----------------------------------------------------------------------------
// Is called whenever a key is pressed/released via GLFW
//-----------------------------------------------------------------------------
void glfw_onKey(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
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