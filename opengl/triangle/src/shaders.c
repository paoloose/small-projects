#include <stdio.h>
#include <stdlib.h>
#include "shaders.h"
#include "utils.h"

/**
 * Replace the source of the shader and compile it.
 * When GL_COMPILE_STATUS is an error, it prints it.
*/
static GLuint CompileShader(GLuint shader, const char* source);

GLuint CreateShader(ShaderProgramSource shaderSource) {
    // You can think of this like an actual C program
    GLuint program = glCreateProgram();

    // You have the source code of two translation units (vertex and fragment shaders)
    GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);

    // You compile each of them
    if (CompileShader(vshader, shaderSource.vertexShader) != 0) {
        return EXIT_FAILURE;
    }
    if (CompileShader(fshader, shaderSource.fragmentShader) != 0) {
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

static GLuint CompileShader(GLuint shader, const char* source) {
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

#define BUFFER_CHUNK_SIZE 128

struct DynamicBuffer {
    char* data;
    size_t capacity;
    size_t cursor;
};

ShaderProgramSource ParseShaderFile(char* filepath) {
    FILE* shadersFile = fopen(filepath, "r");

    struct DynamicBuffer shaders[2];
    shaders[0].cursor = 0;
    shaders[1].cursor = 0;
    shaders[0].capacity = BUFFER_CHUNK_SIZE;
    shaders[1].capacity = BUFFER_CHUNK_SIZE;
    shaders[0].data = calloc(BUFFER_CHUNK_SIZE, sizeof(char));
    shaders[1].data = calloc(BUFFER_CHUNK_SIZE, sizeof(char));

    char chunk[BUFFER_CHUNK_SIZE];

    enum ShaderType {
        NONE_TYPE = -1,
        VERTEX_TYPE = 0,
        FRAGMENT_TYPE = 1
    } shaderType = NONE_TYPE;

    while (fgets(chunk, BUFFER_CHUNK_SIZE, shadersFile) != NULL) {
        if (strstr(chunk, "#shader fragment")) {
            shaderType = FRAGMENT_TYPE;
        }
        else if (strstr(chunk, "#shader vertex")) {
            shaderType = VERTEX_TYPE;
        }
        else if (shaderType != NONE_TYPE) {
            struct DynamicBuffer* shaderBuffer = &shaders[shaderType];
            char* foundBreak = strchr(chunk, '\n');
            size_t len = (foundBreak == NULL ? BUFFER_CHUNK_SIZE - 1 : (foundBreak - chunk) + 1);
            if (shaderBuffer->cursor + len > shaderBuffer->capacity) {
                // buffer got filled, let's augment it
                shaderBuffer->capacity += BUFFER_CHUNK_SIZE;
                shaderBuffer->data = realloc(
                    shaderBuffer->data,
                    shaderBuffer->capacity
                );
                APP_ASSERT(shaderBuffer->data != NULL, "Failed to allocate memory for shader");
            }
            memcpy(shaderBuffer->data + shaderBuffer->cursor, chunk, len);
            shaderBuffer->cursor += len;
            shaderBuffer->data[shaderBuffer->cursor] = '\0';
        }
    }

    ShaderProgramSource source;
    source.vertexShader = shaders[0].data;
    APP_ASSERT(source.vertexShader != NULL, "Failed to load vertex shader");
    source.fragmentShader = shaders[1].data;
    APP_ASSERT(source.fragmentShader != NULL, "Failed to load fragment shader");
    return source;
}

GLuint LoadShader(char* filepath) {
    ShaderProgramSource source = ParseShaderFile(filepath);
    GLint program = CreateShader(source);
    free(source.vertexShader);
    free(source.fragmentShader);

    return program;
}
