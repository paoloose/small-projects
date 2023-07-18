#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include "shaders.h"
#include "utils.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void error_callback(int error, const char* description);

int main(void) {
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(640, 480, "floating", NULL, NULL);
    APP_ASSERT(window != NULL, "Failed to create GLFW window");

    glfwSetKeyCallback(window, key_callback);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    /*
    From: https://www.glfw.org/docs/3.3/quick.html#quick_include
        If you are using an extension loader library to access modern OpenGL
        then this is when to initialize it, as the loader needs a current
        context to load from. This example uses glad, but the same rule applies
        to all such libraries.
    */
    int version = gladLoadGL(glfwGetProcAddress);
    APP_ASSERT(version != 0, "Failed to initialize OpenGL context");
    printf("OpenGL version: %s\n", glGetString(GL_VERSION));
    printf("glad %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

    // Start 🐢

    float positions[] = {
         0.0f, -0.5f, // first  vertex
        -0.5f,  0.5f, // second vertex
         0.5f,  0.5f  // third  vertex
    };
    // Create a 'buffer name' to work with from our code
    GLuint buffer_name; // our VBO (vertex buffer object)
    // Our GPU will create that buffer in its VRAM, we will only refer to it from its name
    glGenBuffers(1, &buffer_name);
    // Select (bind) the buffer in the OpenGL state
    glBindBuffer(GL_ARRAY_BUFFER, buffer_name); // buffer bound, let's work on it
    // Buffer is empty, no problem. Let's send our `vertex` data to the GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_DYNAMIC_DRAW);

    // As you can see, we send raw bytes to our GPU.
    // It didn't now, however, how to interpret them, nor how to draw it
    // It's just a buffer filled out with data, let's define the attributes

    glEnableVertexAttribArray(0); // enable the 0-index attrib of the bound vertex
    // equivalent to: glEnableVertexArrayAttrib(buffer_name, 0);
    glVertexAttribPointer(
        0, // index: the first (and only) attribute
        2, // size: the number of components per attribute type (2 floats)
        GL_FLOAT, // type: the data type of each attribute component
        GL_FALSE, // normalized: no needed for floats
        2 * sizeof(float), // stride: the size between vertices, { float, float }
        0 // pointer: the offset of the attribute we are referring to within a vertex.
          //          we only have one attribute, so offset 0.
          //          For serious cases, you should use offsetof(struct, property)
    );

    GLuint program = LoadShader("./resources/basic.shader");
    APP_ASSERT(program != GL_FALSE, "Failed to create shader program");
    glUseProgram(program);

    // Go to the next '🐢' to see the drawing

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.10f, 0.09f, 0.11f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Here! 🐢
        // This is the draw call for our buffer. Starting from 0, with 3 vertex
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // Our GPU DO knows that we are referring to our buffer,
        // since is currently BOUND, that's how state machines behavior

        // If we run this code, however, we will see nothing. We need to setup
        // a shader to tell our GPU how to draw our data!

        glfwSwapBuffers(window); // Swap front and back buffers
        glfwPollEvents(); // Poll for and process events
    }

    glfwTerminate();
    return 0;
}

void error_callback(int error, const char* description) {
    (void)error;
    fprintf(stderr, "Error 🤭: %s\n", description);
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    (void)scancode;
    (void)mode;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}
