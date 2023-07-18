#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>

typedef struct {
    char* vertexShader;
    char* fragmentShader;
} ShaderProgramSource;

/**
 * Abstract the whole process of creating a shader program from a vertex
 * and a fragment shader
*/
GLuint CreateShader(ShaderProgramSource shaderSource);

/**
 * Parses a shader file and split it into vertex and fragment shaders
 * Struct members need to be freed manually
*/
ShaderProgramSource ParseShaderFile(char* filepath);

/**
 * Equivalent to CreateShader, but loads it from a .shader file
*/
GLuint LoadShader(char* filepath);
