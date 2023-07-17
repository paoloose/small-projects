# OpenGL triangle

Kind of excited that a triangle requires so much lines of code and theory to draw:

<!-- markdownlint-disable MD033 -->
<p align="center">
    <img width="300" src="./triangle.png" />
</p>

The following are notes that I took while following the
[OpenGL Cherno series](https://www.youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2). Revisions
are welcome.

## OpenGL

First of all, OpenGL is an specification: a set of defined functions to communicate with your GPU drivers. Rather than code, OpenGL has a well defined API with the necessary methods for creating graphics applications.

**Who actually implements OpenGL**? Now that's the question. And the answer is that GPU manufacturers provide OpenGL implementations as part of their GPU drivers to allow controlling programs to use the GPU.

Basically, you call an OpenGL function -> then your operating system, which implements an specific version of the OpenGL interface, *delegates* the call to the actual OpenGL implementation that comes with your GPU drivers (both those that you download and the built-ins) -> Finally, these implementations get called on your GPU. The behavior is an implementation detail.

> OpenGL interface is supported in almost every operative system.
>
> - In Windows it's called [WGL](https://learn.microsoft.com/en-us/windows/win32/opengl/opengl), and, since they already have its graphics API, DirectX, the built-in support for OpenGL is limited to version 1.1 (we are currently in version > 4).
> - In [Linux](linux.md) based OS, it's called GLX, designed for the X window system (X11).
> - On MacOS X this happens to be the "OpenGL Framework".
>
> However, as said before, this is only another layer of communication and no code is involved yet.
> When you call an OpenGL function, your operating system's OpenGL implementation (4) acts as an intermediary between your application and the GPU's OpenGL implementation (3), which then compile and run the appropiate machine instructions on its hardware and its context (2).
>
> The numbering used above comes from here: <https://stackoverflow.com/questions/6399676/how-does-opengl-work-at-the-lowest-level>

### How to actually use OpenGL

OpenGL is a cross-platform implementation, but many common graphic application tasks such as creating a window are still OS dependent and not handled by OpenGL.

That is, for every operating system your application support you will have to communicate with its native API in order in order to create a window and link an OpenGL context to it.

To go cross-platform with OpenGL luckily there are plenty of solutions. I used **GLFW** which comes as `libglfw3-dev` in apt. It basically manage the task of creating a window, an OpenGL context for it, and including the appropiate OpenGL headers for your environment.

### Using the last OpenGL version that your GPU supports

As said before, GPU drivers are who implement the OpenGL specification, but your OS' implementation may sometimes be outdated and thus will not able to provide you with the last OpenGL features, even when your GPU does support them. So there's a version mismatch here. Windows is the **extreme** example of this problem.

We can do the daring task of manually creating bindings (pointer functions) to the actual driver implementations. and expose them with the appropiate signature and naming.

No. We're not doing that. Solutions like `GLEW` and [`glad`](https://gen.glad.sh/) already solve this problem for us.

> Note: with glad you can get cross platform easily and have all the dependencies locally. `GLEW` was used by Cherno but I preferred to give glad a try.

### Getting started with Modern OpenGL

Key concepts:

- OpenGL is a state machine.
- We send and handle data from the CPU to our GPU through **buffers** and Buffer Objects.
- Buffers need to be created `glGenBuffers` and deleted `glDeleteBuffers` manually. This is the Gen/Delete paradigm.
- Modern OpenGL drawing works as a pipeline called the **rendering pipeline**.

#### Buffer objects

Let's dive into **Buffer Objects**.
Buffer Objects are OpenGL objects that store an array of **unformatted data** in the memory allocated efficiently by the OpenGL context (aka the GPU) and accesible trough identifiers. Buffer Objects are used to store Vertex data, Pixel data retrieved from images, texture, shaders, [etc](https://www.khronos.org/opengl/wiki/Buffer_Object#General_use). Naming convention: `GL_(...)_BUFFER`.

The basic process is:

- You generate the buffer and get its **name**, i.e. and **identifier** for the buffer that lives in the GPU.

    ```c
    GLuint buffer; // names are basically numbers that we can refer to
    glGenBuffers(1, &buffer); // (GLsizei n, GLuint* buffers)
    ```

- When working with the buffer object, you need to first **bind** it to the current context.
 [`glBindBuffer(enum _target_, uint _bufferName_)`](https://www.khronos.org/opengl/wiki/GLAPI/glBindBuffer "GLAPI/glBindBuffer")

    ```c
    // target: how you intend to use the buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    // buffer is currently bound
    ```

- Now let's fill the buffer with data. Buffer objects hold a linear array of memory of arbitrary size. You will need to choose whether you want your data to be allocated **mutable** or **inmutable**. This decision will change how you interact with your buffer.

    ```c
    float positions[] = {
        -0.5f, -0.5f,
        0.0f, 0.5f,
        0.5f, -0.5f
    };

    // Inmutable data
    // It will not able to update, nor reallocate the data
    glBufferStorage(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_MAP_READ_BIT);

    // Mutable data
    // Buffer can be updated and reallocated
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_DYNAMIC_DRAW);
    ```

- Data operations: As an example, *mutable* Object Buffer's data can be updated with another `glBufferData` call. However, this will reallocate your buffer; and for inmutable data it will do nothing.
 You can perform:
  - Clearing
  - Copying
  - Mapping
  - Invalidation
  - Streaming
 See: <https://www.khronos.org/opengl/wiki/Buffer_Object#Data_Specification>

#### Vertex Buffer Object

Most useful buffer objects for drawing are **Vertex Buffer Objects**. VBO can be packed in many ways. You'll need to think about vertex as a set of user-defined schemes of data.

Vertices are points in your geometry:

```c
// A vertex is not a position! Vertices *can* contain way more data.
struct Vertex {
 GLfloat position[3];
 GLfloat normal[3];
 Glubyte color[4];
};
```

And we use `glVertexAttribPointer` to refer and update the attributes of our vertices.

```c
glVertexAttribPointer(
    GLuint index,
    GLint size,
    GLenum type,
    GLboolean normalized,
    GLsizei stride,
    const GLvoid* pointer
);
```

> - `GLuint index`: the attribute of the index we are referring to.
> - `size`: the number of components per generic vertex attribute. Must be 1, 2, 3, 4. For instance, if we want to represent a 2D position the size will be two and the type `GL_FLOAT`.
> - `type`: the data type of each component.
> - `normalized`: whether we want the GPU to normalize our data. No needed for `GL_FLOAT`.
> - `stride`: the size of each vertex, that is, the sum of all its components size.
> - `pointer`: the bytes offset of the attribute within a single vertex. Offsets are numbers. Conceptually pointers (as seen in the prototype), but still numbers.

The following code should be in the main function, before starting the main loop:

```c
float positions[] = {
    -0.5f, -0.5f, // first vertex
    0.0f,  0.5f, // second vertex
    0.5f, -0.5f  // third vertex
};

// Create a 'buffer name' to work with from our code
GLuint buffer_name;

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
    // we only have one attribute, so offset 0.
    // For serious cases, you should use offsetof(struct, property)
);
```

At this point, we have completed successfully the **Vertex specification** phase! Let's see what left in the pipeline to actually see our triangle!

![OpenGL pipeline](https://www.khronos.org/opengl/wiki_opengl/images/RenderingPipeline.png)

> Holy fuck.

#### Writing a shader

There are two types of shaders in OpenGL: Vertex Shaders and Fragment Shaders.
The specification says that in order for the GPU to know how to draw or VBO we need to **define** a shader, that is, programmable code that runs in the GPU.

However, if you have executed `glDrawArrays(GL_TRIANGLES, 0, 3);`  (with the vertex bound) you probably did see your actual being drawn. Some GPU manufacturers implement some basic default shaders. However we should not rely on them since may vary between users.

Key concepts:

- A vertex shader is called one time **for each vertex** of your geometry.
- A vertex shader will tell your GPU **where** you want your triangle to be in the screen. Vertex shaders have access to the vertex attributes, and can make operations based on its values. Think about cameras.
- Fragment shader get called one time **for each pixel** that gets rasterized while drawing your triangle.
- Fragment shaders basically define **the color** output of each pixel. Think about lighting as an example.

This is were the real fun begins. *Shaders are basically the 90% of the graphics world*. When you have the power of calculating the color for each pixel of your geometry, possibilities increase dramatically.

Really cool shaders get thousands of lines of code. Game engines actually generates shaders on the fly based on what's happening in your game.

So let's write the most basic shader program possible:

- a Vertex Shader to tell the GPU to use the same position as we define in our
  vertices;
- a Fragment Shader to tell the GPU to use a solid color for each pixel.

For writing shaders we use GLSL (OpenGL Shader Language). Here is how the code looks like:

Is worth to remember how we defined each of our vertices:

```c
float positions[] = {
    -0.5f, -0.5f, // first vertex
     0.0f,  0.5f, // second vertex
     0.5f, -0.5f  // third vertex
};

// buffer creation, binding and data sending
// ...

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

```

So the vertex shader would look like this:

```glsl
#version 330 core
layout(location = 0) in vec2 position;

void main() {
    gl_Position = vec4(position.x, position.y, 1.0, 1.0);
}
```

And a solid red color fragment shader:

```glsl
#version 330 core
layout(location = 0) out vec4 color;

void main() {
    color = vec4(0.5, 0.0, 0.0, 1.0);
}
```

You probably think this is C code, but this GLSL will be sent to the GPU
as a string. The GPU is the responsible of compiling, linking and and executing it.

Two functions were defined in `shaders.c` to abstract this process:

```c
#include <stdio.h>
#include <stdlib.h>
#include "shaders.h"

GLuint CompileShader(GLuint shader, const char* source) {
    const GLsizei shaders_count = 1;
    const GLint* source_len = NULL; // NULL means source is null terminated!
    glShaderSource(shader, shaders_count, &source, source_len);

    // Source code is now in the GPU, let's compile it
    glCompileShader(shader);

    // Let's make some compilation error handling
    // Remember that OpenGL fails silently by default

    // How we access the shader status?
    // We use the `getShaderiv` function
    // Basically we will check first is there are any errors,
    // and if so, we will print the error log

    // glGetShaderiv — Returns a parameter from a shader object

    GLint result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result != GL_TRUE) {
        int log_len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);
        char* message = alloca(log_len * sizeof(char));
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
```

Now defining the source, compiling, linking, and creating a shader program is as easy as:

```c
char* vertexShader =
    "#version 330 core\n"
    "\n"
    "layout (location = 0) in vec2 position;\n"
    "\n"
    "void main() {\n"
    // gl_Position expects a vec4, so you have to cast it implicitly (up) or explicitly (here)
    "   gl_Position = vec4(position.x, position.y, 0.0, 1.0);\n"
    "}";

char* fragmentShader =
    "#version 330 core\n"
    "\n"
    "layout (location = 0) out vec4 color;\n"
    "\n"
    "void main() {\n"
    "   color = vec4(0.5, 0.0, 0.0, 1.0);\n"
    "}";

GLuint program = CreateShader(vertexShader, fragmentShader);
if (program == GL_FALSE) {
    glfwTerminate();
    return -1;
}
glUseProgram(program);
```

And now we can see our triangle in red!

```c
while (!glfwWindowShouldClose(window)) {
    // ...
    glDrawArrays(GL_TRIANGLES, 0, 3);
    // ...
}
```

![My triangle](./triangle.png)

#### Acknowledgements

- TheCherno Youtube Channel: <https://www.youtube.com/@TheCherno>
- VictorGordan Youtube Channel: <https://www.youtube.com/@VictorGordan>
- Khronos documentation: <https://www.khronos.org/opengl/wiki/Vertex_Specification>
