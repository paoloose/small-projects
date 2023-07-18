#shader vertex

#version 330 core

layout (location = 0) in vec2 position;

void main() {
// gl_Position expects a vec4, so you have to cast it implicity (up) or explicitly (here)
   gl_Position = vec4(position.x, position.y, 0.0, 1.0);
};

#shader fragment

#version 330 core

layout (location = 0) out vec4 color;

void main() {
   color = vec4(0.57, 0.50, 0.74, 1.0);
};

