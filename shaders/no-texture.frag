#version 330 core

in vec2 UV;
in vec3 Normal;
in vec3 FragPos;

out vec3 color;

uniform vec3 lightPos;
uniform vec3 viewPos;

float ambinatStrength = 0.1;
vec3 lightColor = vec3(2);
float specularStrength = 0.1;
vec3 baseColor = vec3(1);


void main()
{

    vec3 ambiant = ambinatStrength * lightColor;
    

    vec3 norm = normalize(Normal);

    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = min(max(dot(norm, lightDir), 0.0), 0.2);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 2);
    vec3 specular = specularStrength * spec * lightColor;
    vec3 result = (ambiant + diffuse + specular) * baseColor;

    color = vec3(result);
}