#include <glad/gl.h>
#include <GLFW/glfw3.h>

/**
 * Replace the source of the shader and compile it.
*/
GLuint CompileShader(GLuint shader, const char* source);

GLuint CreateShader(const char* vertexShader, const char* fragmentShader);
