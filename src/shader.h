#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;  

class Shader
{
    public:
        unsigned int ID;

        Shader(const char* vertexPath, const char* fragmentPath)
        {
            // Retrieve the vertex and fragment source code from file paths.
            string vertexCode;
            string fragmentCode;
            ifstream vShaderFile;
            ifstream fShaderFile;

            // Ensure ifstream objects can throw exceptions.
            vShaderFile.exceptions (ifstream::failbit | ifstream::badbit);
            fShaderFile.exceptions (ifstream::failbit | ifstream::badbit);

            try 
            {
                // Open files.
                vShaderFile.open(vertexPath);
                fShaderFile.open(fragmentPath);
                stringstream vShaderStream, fShaderStream;

                // Read files' buffer contents into streams.
                vShaderStream << vShaderFile.rdbuf();
                fShaderStream << fShaderFile.rdbuf();

                // Close file handlers.
                vShaderFile.close();
                fShaderFile.close();

                // Convert stream into string.
                vertexCode = vShaderStream.str();
                fragmentCode = fShaderStream.str();
            }
            catch (ifstream::failure& e)
            {
                cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
            }

            const char* vShaderCode = vertexCode.c_str();
            const char * fShaderCode = fragmentCode.c_str();

            // Compile shaders.
            unsigned int vertex, fragment;

            // Create a vertex shader to calculate 3D coordinates.
            vertex = glCreateShader(GL_VERTEX_SHADER);
            // Attach vertex shader definition.
            glShaderSource(vertex, 1, &vShaderCode, NULL);
            // Compile vertex shader.
            glCompileShader(vertex);
            // Check for shader compile errors.
            checkCompileErrors(vertex, "VERTEX");

            // Create a fragment shader to calculate color output of pixels.
            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            // Attach vertex shader definition.
            glShaderSource(fragment, 1, &fShaderCode, NULL);
            // Compile vertex shader.
            glCompileShader(fragment);
            // Check for shader compile errors.
            checkCompileErrors(fragment, "FRAGMENT");

            // Create a shader program to link multiple shaders.
            ID = glCreateProgram();
            glAttachShader(ID, vertex);
            glAttachShader(ID, fragment);
            glLinkProgram(ID);
            // Check for shader program linking errors.
            checkCompileErrors(ID, "PROGRAM");

            // Delete linked shaders.
            glDeleteShader(vertex);
            glDeleteShader(fragment);
        }

        Shader(const char* vertexPath, const char* fragmentPath, const char* texturePath)
        {
            // Retrieve the vertex and fragment source code from file paths.
            string vertexCode;
            string fragmentCode;
            ifstream vShaderFile;
            ifstream fShaderFile;

            // Ensure ifstream objects can throw exceptions.
            vShaderFile.exceptions (ifstream::failbit | ifstream::badbit);
            fShaderFile.exceptions (ifstream::failbit | ifstream::badbit);

            try 
            {
                // Open files.
                vShaderFile.open(vertexPath);
                fShaderFile.open(fragmentPath);
                stringstream vShaderStream, fShaderStream;

                // Read files' buffer contents into streams.
                vShaderStream << vShaderFile.rdbuf();
                fShaderStream << fShaderFile.rdbuf();

                // Close file handlers.
                vShaderFile.close();
                fShaderFile.close();

                // Convert stream into string.
                vertexCode = vShaderStream.str();
                fragmentCode = fShaderStream.str();
            }
            catch (ifstream::failure& e)
            {
                cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
            }

            const char* vShaderCode = vertexCode.c_str();
            const char * fShaderCode = fragmentCode.c_str();

            // Compile shaders.
            unsigned int vertex, fragment;

            // Create a vertex shader to calculate 3D coordinates.
            vertex = glCreateShader(GL_VERTEX_SHADER);
            // Attach vertex shader definition.
            glShaderSource(vertex, 1, &vShaderCode, NULL);
            // Compile vertex shader.
            glCompileShader(vertex);
            // Check for shader compile errors.
            checkCompileErrors(vertex, "VERTEX");

            // Create a fragment shader to calculate color output of pixels.
            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            // Attach vertex shader definition.
            glShaderSource(fragment, 1, &fShaderCode, NULL);
            // Compile vertex shader.
            glCompileShader(fragment);
            // Check for shader compile errors.
            checkCompileErrors(fragment, "FRAGMENT");

            // Create a texture.
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            // Set the texture wrapping parameters.
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // Set texture filtering parameters.
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // Load image, create texture and generate mipmaps.
            int width, height, nrChannels;
            unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            else
            {
                cout << "Failed to load texture." << endl;
            }
            stbi_image_free(data);

            // Bind texture.
            glBindTexture(GL_TEXTURE_2D, texture);

            // Create a shader program to link multiple shaders.
            ID = glCreateProgram();
            glAttachShader(ID, vertex);
            glAttachShader(ID, fragment);
            glLinkProgram(ID);
            // Check for shader program linking errors.
            checkCompileErrors(ID, "PROGRAM");

            // Delete linked shaders.
            glDeleteShader(vertex);
            glDeleteShader(fragment);
        }

        void use()
        {
            // Activate shader program.
            glUseProgram(ID);
        }

        // Utility uniform functions.
        void setBool(const string &name, bool value) const
        {         
            glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
        }

        void setInt(const string &name, int value) const
        { 
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
        }

        void setFloat(const string &name, float value) const
        { 
            glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
        }

        void setVec2(const string &name, const glm::vec2 &value) const
        { 
            glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
        }

        void setVec2(const string &name, float x, float y) const
        { 
            glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y); 
        }

        void setVec3(const string &name, const glm::vec3 &value) const
        { 
            glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
        }
        
        void setVec3(const string &name, float x, float y, float z) const
        { 
            glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); 
        }

        void setVec4(const string &name, const glm::vec4 &value) const
        { 
            glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
        }

        void setVec4(const string &name, float x, float y, float z, float w) const
        { 
            glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); 
        }

        void setMat2(const string &name, const glm::mat2 &mat) const
        {
            glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }

        void setMat3(const string &name, const glm::mat3 &mat) const
        {
            glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }

        void setMat4(const string &name, const glm::mat4 &mat) const
        {
            glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
    
    private:
        void checkCompileErrors(unsigned int shader, string type)
        {
            int success;
            char infoLog[1024];
            if (type != "PROGRAM")
            {
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
                if (!success)
                {
                    glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                    cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n" << endl;
                }
            }
            else
            {
                glGetProgramiv(shader, GL_LINK_STATUS, &success);
                if (!success)
                {
                    glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                    cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n" << endl;
                }
            }
        }

};
#endif