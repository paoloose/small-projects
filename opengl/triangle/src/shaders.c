#include <stdio.h>
#include <stdlib.h>
#include "shaders.h"

GLuint CompileShader(GLuint shader, const char* source) {
    const GLsizei shaders_count = 1;
    const GLint* source_len = NULL; // source should be null terminated!
    glShaderSource(shader, shaders_count, &source, source_len);

    glCompileShader(shader);

    // Let's make some compilation error handling
    // Remember that OpenGL fails silently by default
    GLint result;
    // glGetShaderiv — Returns a parameter from a shader object
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result != GL_TRUE) {
        int log_len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);
        char* message = alloca(log_len * sizeof(char)); // NOTE: alloca used uwu
        glGetShaderInfoLog(shader, log_len, &log_len, message);

        fprintf(
            stderr,
            "Failed to compile %s shader %s",
            shader == GL_VERTEX_SHADER ? "vertex" : "fragment",
            message
        );
        glDeleteShader(shader);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

GLuint CreateShader(const char* vertexShader, const char* fragmentShader) {
    // You can think of this like an actual C program
    GLuint program = glCreateProgram();

    // You have the source code of two translation units (vertex and fragment shaders)
    GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);

    // You compile each of them
    if (CompileShader(vshader, vertexShader) != 0) {
        return EXIT_FAILURE;
    }
    if (CompileShader(fshader, fragmentShader) != 0) {
        return EXIT_FAILURE;
    }

    // And you link them together in your program
    glAttachShader(program, vshader);
    glAttachShader(program, fshader);
    glLinkProgram(program);

    // After linking, you can delete the shaders
    // They are already linked in the program and we don't need them anymore
    glDetachShader(program, vshader);
    glDetachShader(program, fshader);

    glValidateProgram(program);

    glDeleteShader(vshader);
    glDeleteShader(fshader);

    return program;
}
