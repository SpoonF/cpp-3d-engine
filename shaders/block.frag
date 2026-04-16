#version 330 core

in vec2 UV;
in vec3 Normal;
in vec3 FragPos;

out vec3 color;

uniform sampler2D textureSampler;
uniform int hasTexture;
uniform vec3 lightPos;
uniform vec3 viewPos;

float ambinatStrength = 0.3;
vec3 lightColor = vec3(2.55, 2.55, 2.55);
float specularStrength = 0.4;


void main()
{
    vec3 textur = texture(textureSampler, UV).rgb;


    vec3 ambiant = ambinatStrength * lightColor;

    // vec3 result = ambiant * textur;
    // color = vec3(result);
    

    vec3 norm = normalize(Normal);

    // vec3 lightDir = normalize(lightPos - FragPos);
    vec3 lightDir = normalize(vec3(54,34,65) - FragPos);

    float diff = min(max(dot(norm, lightDir), 0.0), 0.2);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;
    vec3 result = (ambiant + diffuse + specular) * textur;

    color = vec3(result);


}