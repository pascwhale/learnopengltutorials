/*
 * Sources
 * https://learnopengl.com/Getting-started/Hello-Window
 * https://learnopengl.com/Getting-started/Hello-Triangle
 * https://learnopengl.com/Getting-started/Shaders
 * https://learnopengl.com/Getting-started/Textures
 * https://learnopengl.com/Getting-started/Transformations
 * https://learnopengl.com/Getting-started/Coordinate-Systems
 * https://learnopengl.com/Getting-started/Camera
 * https://learnopengl.com/Lighting/Basic-Lighting
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
#include "camera.h"

// Adjust viewport on window resize.
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Process mouse and scroll wheel movement.
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// Detect user inputs.
void processInput(GLFWwindow *window);

// Define screen width and height.
const unsigned int SCREEN_WIDTH = 960;
const unsigned int SCREEN_HEIGHT = 540;

// Initialize variables used to process mouse movement.
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;

// Define variable to keep track of time.
float currentFrame;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Create a canera object.
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

// Define variables for light object.
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

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

    // Capture mouse an scroll wheel movement.
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
    
    // Initialize GLEW.
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW." << std::endl;
        glfwTerminate();
        return -1;
    }

    const char* vertexShaderPath = "../shaders/basic_lighting_vertex_shader.txt";
    const char* fragmentShaderPath = "../shaders/basic_lighting_fragment_shader.txt";
    const char* lightCubeVertexShaderPath = "../shaders/light_cube_vertex_shader.txt";
    const char* lightCubeFragmentShaderPath = "../shaders/light_cube_fragment_shader.txt";
    Shader light_shader_program(vertexShaderPath, fragmentShaderPath);
    Shader light_cube_shader_program(lightCubeVertexShaderPath, lightCubeFragmentShaderPath);

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind vertex buffer.
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    // Unbind vertex array.
    glBindVertexArray(0);

    // Create a vertex array for the light cube.
    GLuint lightVertexArrayObject;
    glGenVertexArrays(1, &lightVertexArrayObject);
    glBindVertexArray(lightVertexArrayObject);

    // Create a vertex buffer for the light cube.
    GLuint lightVertexBufferObject;
    glGenBuffers(1, &lightVertexBufferObject);
    
    // Copy vertices in vertex buffer for the light cube.
    glBindBuffer(GL_ARRAY_BUFFER, lightVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Set the vertex attributes pointers for the light cube.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind vertex buffer.
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    // Unbind vertex array.
    glBindVertexArray(0); 

    // Enable depth testing.
    glEnable(GL_DEPTH_TEST);

    // Entering Main Loop.
    while(!glfwWindowShouldClose(window))
    {

        // Update variables that keep track of time.
        currentFrame = (float) glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Set default pixel color.
        glClearColor(0.1, 0.1, 0.1, 1.0f);

        // Clear color an depth buffer.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Activate shader program.
        light_shader_program.use();
        light_shader_program.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        light_shader_program.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        light_shader_program.setVec3("lightPos", lightPos);
        light_shader_program.setVec3("viewPos", camera.Position);

        // Define view and projection transformations.
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        light_shader_program.setMat4("projection", projection);
        light_shader_program.setMat4("view", view);

        // Define world transformation.
        glm::mat4 model = glm::mat4(1.0f);
        light_shader_program.setMat4("model", model);

        // Render an object.
        glBindVertexArray(vertexArrayObject);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        // Draw light cube.
        light_cube_shader_program.use();
        light_cube_shader_program.setMat4("projection", projection);
        light_cube_shader_program.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        light_cube_shader_program.setMat4("model", model);

        glBindVertexArray(lightVertexArrayObject);
        glDrawArrays(GL_TRIANGLES, 0, 36);

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

// Adjust viewport on window resize.
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Process mouse movement.
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// Process scroll wheel movement.
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll((float)yoffset);
}

// Process user inputs.
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(DOWN, deltaTime);
    }
    
}