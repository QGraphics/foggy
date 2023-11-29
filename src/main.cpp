//-----------------------------------------------------------------------------
#include <iostream>
#include <sstream>

#define GLEW_STATIC

#include "GL/glew.h"    // Important - this header must come before glfw3 header
#include "GLFW/glfw3.h"

//textrue rendering
#include "rendering/ShaderProgram.h"
#include "rendering/Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "rendering/Camera.h"

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
FPSCamera fpsCamera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f));
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

    // Set up our quad
    // 1. Set up an array of vertices for a quad (2 triangls) with an index buffer data
    //   (What is a vertex?)
    GLfloat vertices[] = {
            // position		 // tex coords

            // front face
            -1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 1.0f, 1.0f, 0.0f,

            // back face
            -1.0f, 1.0f, -1.0f, 0.0f, 1.0f,
            1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, -1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, -1.0f, 1.0f, 0.0f,

            // left face
            -1.0f, 1.0f, -1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 1.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, -1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
            -1.0f, -1.0f, 1.0f, 1.0f, 0.0f,

            // right face
            1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
            1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, -1.0f, 1.0f, 0.0f,

            // top face
            -1.0f, 1.0f, -1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, -1.0f, 0.0f, 1.0f,
            -1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

            // bottom face
            -1.0f, -1.0f, 1.0f, 0.0f, 1.0f,
            1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
            1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
    };

    glm::vec3 cubePos = glm::vec3(-0.0f, -0.0f, 0.5f);
//    glm::vec3 floorPos = glm::vec3(0.0f, -1.0f, 0.0f);


    // 2. Set up buffers on the GPU
    GLuint vbo, vao;

    glGenBuffers(1, &vbo);                    // Generate an empty vertex buffer on the GPU
    glBindBuffer(GL_ARRAY_BUFFER, vbo);        // "bind" or set as the current buffer we are working with
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);    // copy the data from CPU to GPU

    glGenVertexArrays(1, &vao);                // Tell OpenGL to create new Vertex Array Object
    glBindVertexArray(vao);                    // Make it the current one

    //position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          NULL);    // Define a layout for the first vertex buffer "0"
    glEnableVertexAttribArray(0);            // Enable the first attribute or attribute "0"
    //texture
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (GLvoid *) (3 * sizeof(float)));    // Define a layout for the first vertex buffer "0"
    glEnableVertexAttribArray(1);            // Enable the second attribute or attribute "1"

    //call shaders
    ShaderProgram shaderProgram;
    shaderProgram.loadShaders("assets/shaders/transform.vert", "assets/shaders/transform.frag");

    //****** Texture ******
    Texture texture1;
    texture1.loadTexture("assets/images/cubeTest.jpg", true);

//    Texture floorTexture;
//    floorTexture.loadTexture(floorImage, true);

    double lastTime = glfwGetTime();
    float cubeAngle = 0.0f;

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

        //bind texture
        texture1.bind(1);
        glUniform1i(glGetUniformLocation(shaderProgram.getProgram(), "texSampler1"), 1);


        //set uniform for color with some fun
        GLfloat time = (GLfloat) glfwGetTime();
        GLfloat greenColor = (sin(time) / 2) + 0.5;
        shaderProgram.setUniform("vertColor", glm::vec4(1.0f, greenColor, 0.0f, 1.0f));

        //GLint vertexColorLocation = glGetUniformLocation(shaderProgram.getProgram(), "vertColor");
        //glUniform4f(vertexColorLocation, 0.0f, greenColor, 0.0f, 0.0f);

        //translating
        if (transDirection)
            offset += increment;
        else
            offset -= increment;

        if (abs(offset) >= maxOffset) {
            transDirection = !transDirection;
        }

        //rotation
        curAngle += 0.5f;
        if (curAngle >= 360)
            curAngle -= 360;


        //scale
        if (sizeDirection)
            curSize += 0.001f;
        else
            curSize -= 0.001f;

        if (curSize >= maxSize || curSize <= minSize) {
            sizeDirection = !sizeDirection;
        }

        cubeAngle -= (float) (deltatime * 50.0f);
        if (cubeAngle >= 360)
            cubeAngle = 0.0f;

        //add transformation
//        glm::mat4 transform = glm::mat4(1.0f);
        glm::mat4 projection(1.0f), view(1.0f), model(1.0f);

        //rotate the cube around it's center
//        model = glm::translate(model, cubePos) *
//                glm::rotate(model, glm::radians(cubeAngle), glm::vec3(0.0f, 1.0f, 0.0f));

        model = glm::translate(model, cubePos);


        //LookAt
//        glm::vec3 camPos(0.0f, 0.0f, 0.0f);
//        glm::vec3 targetPos(0.0f, 0.0f, -1.0f);
//        glm::vec3 up(0.0f, 1.0f, 0.0f);

        //Create the view matrix
        view = fpsCamera.getViewMatrix();

        //lookup matrix
//        view = glm::lookAt(camPos, targetPos, up);

        //project matrix
        projection = glm::perspective(glm::radians(fpsCamera.getFOV()), (float) gWindowHeight / (float) gWindowWidth,
                                      0.1f, 100.0f);

        //pass the matrices to the shader
        shaderProgram.setUniform("model", model);
        shaderProgram.setUniform("view", view);
        shaderProgram.setUniform("projection", projection);

//        transform = glm::scale(transform, glm::vec3(0.4f, 0.4f, 0.4f));
//        transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
//        transform = glm::rotate(transform, glm::radians(cubeAngle), glm::vec3(1.0f, 1.0f, 0.0f));
//        shaderProgram.setUniform("transform", transform);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //Create a Floor
//        floorTexture.bind(0);
//        model = glm::translate(model, floorPos) * glm::scale(model, glm::vec3(10.0f, 0.01f, 10.0f));
//        shaderProgram.setUniform("model", model);
//        glDrawArrays(GL_TRIANGLES, 0, 36);
//        glBindVertexArray(0);

        // Swap front and back buffers
        glfwSwapBuffers(gWindow);
        lastTime = currentTime;
    }

    // Clean up
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

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

void glfw_onMouseScroll(GLFWwindow *window, double deltaX, double deltaY) {
    double fov = fpsCamera.getFOV() + deltaY * ZOOM_SENSITIVITY;
    fov = glm::clamp(fov, 1.0, 120.0);
    fpsCamera.setFOV((float) fov);
}

void update(double elapseTime) {
    //camer orientation
    double mouseX, mouseY;

    //get current mouse cursor position delta
    glfwGetCursorPos(gWindow, &mouseX, &mouseY);

    //rotate the camera using the dlta in mouse differences
    fpsCamera.rotate(float(gWindowWidth / 2.0f - mouseX) * MOUSE_SENSETIVITY,
                     float(gWindowHeight / 2.0f - mouseY) * MOUSE_SENSETIVITY);

    glfwSetCursorPos(gWindow, gWindowWidth / 2.0, gWindowHeight / 2.0);

    //camera move forwad backword
    if (glfwGetKey(gWindow, GLFW_KEY_W) == GLFW_PRESS)
        fpsCamera.move((float) MOVE_SPEED * (float) elapseTime * fpsCamera.getLook());
    else if (glfwGetKey(gWindow, GLFW_KEY_S) == GLFW_PRESS)
        fpsCamera.move((float) MOVE_SPEED * (float) elapseTime * -fpsCamera.getLook());

    //left right
    if (glfwGetKey(gWindow, GLFW_KEY_A) == GLFW_PRESS)
        fpsCamera.move((float) MOVE_SPEED * (float) elapseTime * -fpsCamera.getRight());
    else if (glfwGetKey(gWindow, GLFW_KEY_D) == GLFW_PRESS)
        fpsCamera.move((float) MOVE_SPEED * (float) elapseTime * fpsCamera.getRight());

    //up down
    if (glfwGetKey(gWindow, GLFW_KEY_Z) == GLFW_PRESS)
        fpsCamera.move((float) MOVE_SPEED * (float) elapseTime * fpsCamera.getUp());
    else if (glfwGetKey(gWindow, GLFW_KEY_X) == GLFW_PRESS)
        fpsCamera.move((float) MOVE_SPEED * (float) elapseTime * -fpsCamera.getUp());
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