#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;

uniform float off;

void main() {
    vec3 newPos = vec3(aPos.x + off, aPos.yz);
    gl_Position = vec4(newPos, 1.0);
    ourColor = newPos;
}

