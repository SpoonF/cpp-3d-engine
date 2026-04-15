#version 330 core
layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec2 vertUV;
layout (location = 2) in vec3 vertOffset;
layout (location = 3) in vec3 vertSizes;
layout (location = 4) in vec3 normal;

out vec2 UV;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;
uniform mat3 ModelNormal;
mat4 MVP;
vec3 aPos;

void main()
{
    MVP = Projection * View * Model;
    aPos = vertPos + vertOffset;
    gl_Position = MVP * vec4(aPos, 1.0);
    UV = vertUV;

    FragPos = vec3(Model * vec4(vertPos + vertOffset, 1.0));
    Normal =  normal;
}