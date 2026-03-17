#version 330 core
in vec2 UV;
out vec4 color;

uniform sampler2D textureSampler;

float zNear = 0.1;
float zFar = 100.0;

float LinearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0;
    return (2.0 * zNear * zFar) / (zFar + zNear - z * (zFar - zNear));
}

void main()
{
    vec3 textur = texture(textureSampler, UV).rgb;
    // textur.z = LinearizeDepth(textur.z) / zFar;
    color = vec4(textur, 1.0);
}