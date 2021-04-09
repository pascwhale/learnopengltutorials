/*
 * Sources
 * https://learnopengl.com/Getting-started/Hello-Window
 * https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/1.2.hello_window_clear/hello_window_clear.cpp
 * https://learnopengl.com/Getting-started/Hello-Triangle
 * https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/2.1.hello_triangle/hello_triangle.cpp
 * https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/2.2.hello_triangle_indexed/hello_triangle_indexed.cpp
 */

#include <iostream>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL.
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
        std::cerr << "Failed to create GLEW." << std::endl;
        glfwTerminate();
        return -1;
    }

    // Define vertex shader.
    const char *vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aColor;\n"
        "out vec3 vertexColor;\n"
        "void main()\n"
        "{\n"
        "   vertexColor = aColor;\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0"; 

    // Create a vertex shader to calculate 3D coordinates.
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Attach vertex shader definition.
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // Compile vertex shader.
    glCompileShader(vertexShader);
    // Check for shader compile errors.
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Define fragment shader.
    const char *fragmentShaderSource = "#version 330 core\n"
        "in vec3 vertexColor;\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(vertexColor.x, vertexColor.y, vertexColor.z, 1.0f);\n"
        "}\n\0";

    // Create a fragment shader to calculate color output of pixels.
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // Attach vertex shader definition.
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    // Compile vertex shader.
    glCompileShader(fragmentShader);
    // Check for shader compile errors.
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
        
    // Create a shader program to link multiple shaders.
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Check for shader program linking errors.
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Delete linked shaders.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Create vertex data for rectangle.
    float vertices[] = {
        -0.5f,  -0.5f,  0.0f,
        1.0f,   0.0f,   0.0f,
        0.5f,   -0.5,   0.0f,
        0.0f,   1.0f,   0.0f,
        0.0f,    0.5,   0.0f,
        0.0f,   0.0f,   1.0f
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

    // Set the vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); 

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind vertex buffer.
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    // Unbind vertex array.
    glBindVertexArray(0);

 
    
    // Entering Main Loop
    while(!glfwWindowShouldClose(window))
    {
        // Set default pixel color.
        glClearColor(0.75f, 0.75f, 0.75f, 1.0f);

        // Reset color of each pixel to glClearColor.
        glClear(GL_COLOR_BUFFER_BIT);

        // Activate shader program.
        glUseProgram(shaderProgram);
        glBindVertexArray(vertexArrayObject);
        glDrawArrays(GL_TRIANGLES, 0, 3);

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
