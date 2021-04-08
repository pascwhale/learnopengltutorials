/*
 * Sources
 * https://learnopengl.com/Getting-started/Hello-Window
 * https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/1.2.hello_window_clear/hello_window_clear.cpp
 */

#include <iostream>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL .
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler.
#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
                        // initializing OpenGL and binding inputs.
#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language.

// Adjust viewport on window resize.
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  

// Detect user inputs.
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

const unsigned int SCREEN_WIDTH = 960;
const unsigned int SCREEN_HEIGHT = 540;

int main(int argc, char*argv[])
{
    // Initialize GLFW and OpenGL version.
    glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create window and rendering context using GLFW.
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "COMP 371", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Set window as current context.
    glfwMakeContextCurrent(window);

    // Call viewport resizing function on every window resize.
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // Initialize GLEW.
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Set default pixel color.
    glClearColor(0.75f, 0.75f, 0.75f, 1.0f);

    // Entering Main Loop
    while(!glfwWindowShouldClose(window))
    {
        // Reset color of each pixel to glClearColor.
        glClear(GL_COLOR_BUFFER_BIT);

        // Process user input.
        processInput(window);

        // End frame.
        glfwSwapBuffers(window);
        
        // Detect inputs.
        glfwPollEvents();
    }
    
    // Shutdown GLFW.
    glfwTerminate();
    
	return 0;
}
