#version 460 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;

void main()
{
    vec4 pos = model * vec4(aPos, 0.0, 1.0);
    gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);
    TexCoords = aTexCoords;
}
