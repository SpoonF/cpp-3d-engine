#version 330 core
layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec2 vertUV;
layout (location = 2) in vec3 vertOffset;

out vec2 UV;
uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(vertPos + vertOffset, 1.0);
    UV = vertUV;
}