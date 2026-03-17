#version 330 core
in vec2 UV;
out vec4 color;

uniform sampler2D textureSampler;

void main()
{
    vec3 textur = texture(textureSampler, UV).rgb - vec3(.04);
    color = vec4(textur, 1.0);
}