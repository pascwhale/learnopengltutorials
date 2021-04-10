/*
 * Sources
 * https://learnopengl.com/Getting-started/Hello-Window
 * https://learnopengl.com/Getting-started/Hello-Triangle
 * https://learnopengl.com/Getting-started/Shaders
 * https://learnopengl.com/Getting-started/Textures
 * https://learnopengl.com/Getting-started/Transformations
 * https://learnopengl.com/Getting-started/Coordinate-Systems
 */

#include <iostream>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL.
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler.
#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
                        // initializing OpenGL and binding inputs.
#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language.
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shader.h"

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
        std::cerr << "Failed to create GLEW." << std::endl;
        glfwTerminate();
        return -1;
    }

    const char* vertexShaderPath = "../shaders/basic_vertex_shader.txt";
    const char* fragmentShaderPath = "../shaders/basic_fragment_shader.txt";
    const char* texturePath = "../textures/wall.jpg";
    Shader shader_program(vertexShaderPath, fragmentShaderPath, texturePath);

    float vertices[] = {
       -0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,   -1.0f, -1.0f,
        0.5f, -0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    1.0f, -1.0f,
        0.5f,  0.5f, -0.5f,    0.0f,  0.0f,  1.0f,    1.0f,  1.0f,
        0.5f,  0.5f, -0.5f,    0.0f,  0.0f,  1.0f,    1.0f,  1.0f,
       -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,   -1.0f,  1.0f,
       -0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,   -1.0f, -1.0f,
     
       -0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,   -1.0f, -1.0f,
        0.5f, -0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    1.0f, -1.0f,
        0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f,  1.0f,
       -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,   -1.0f,  1.0f,
       -0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,   -1.0f, -1.0f,
     
       -0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
       -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    1.0f, -1.0f,
       -0.5f, -0.5f, -0.5f,    0.0f,  0.0f,  1.0f,   -1.0f, -1.0f,
       -0.5f, -0.5f, -0.5f,    0.0f,  0.0f,  1.0f,   -1.0f, -1.0f,
       -0.5f, -0.5f,  0.5f,    0.0f,  1.0f,  0.0f,   -1.0f,  1.0f,
       -0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
     
        0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
        0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    1.0f, -1.0f,
        0.5f, -0.5f, -0.5f,    0.0f,  0.0f,  1.0f,   -1.0f, -1.0f,
        0.5f, -0.5f, -0.5f,    0.0f,  0.0f,  1.0f,   -1.0f, -1.0f,
        0.5f, -0.5f,  0.5f,    0.0f,  1.0f,  0.0f,   -1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
       
       -0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,   -1.0f, -1.0f,
        0.5f, -0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    1.0f, -1.0f,
        0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f,  1.0f,
        0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f,  1.0f,
       -0.5f, -0.5f,  0.5f,    0.0f,  1.0f,  0.0f,   -1.0f,  1.0f,
       -0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,   -1.0f, -1.0f,
     
       -0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,   -1.0f, -1.0f,
        0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    1.0f, -1.0f,
        0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f,  1.0f,
       -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,   -1.0f,  1.0f,
       -0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,   -1.0f, -1.0f,
};

    // Create a vertex array.
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    // Create a vertex buffer.
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    
    // Copy vertices in vertex buffer.
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Set the vertex attributes pointers.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); 

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    // Unbind vertex buffer.
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    // Unbind vertex array.
    glBindVertexArray(0);

    // Create transformations.
    // Initialize identity matrix.
    glm::mat4 transform = glm::mat4(1.0f);

    // Create and define transformations for different spaces.
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    float oldTime = (float)glfwGetTime();
    float currentTime = (float)glfwGetTime();
    float deltaTime = currentTime - oldTime;

    float angle = 0.0f;

    // Enable depth testing.
    glEnable(GL_DEPTH_TEST);

    // Entering Main Loop.
    while(!glfwWindowShouldClose(window))
    {
        // Set default pixel color.
        glClearColor(0.75f, 0.75f, 0.75f, 1.0f);

        // Clear color an depth buffer.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Activate shader program.
        shader_program.use();

        // Calculate and apply next angle.
        currentTime = (float)glfwGetTime();
        deltaTime = currentTime - oldTime;
        oldTime = currentTime;
        angle += glm::radians(deltaTime) / 100;
        transform = glm::rotate(transform, angle, glm::vec3(0.0f, 0.0f, 1.0f));
        shader_program.setMat4("transform", transform);
        
        // Send space transformations to shader.
        shader_program.setMat4("model", model);
        shader_program.setMat4("view", view);
        shader_program.setMat4("projection", projection);
        
        // Draw.
        glBindVertexArray(vertexArrayObject);
        glDrawArrays(GL_TRIANGLES, 0, size(vertices)/8);

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
