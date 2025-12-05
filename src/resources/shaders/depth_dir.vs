#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 lightSpaceMatrix;

void main()
{
    // passo de sombra: só manda posição pro espaço da luz
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
}
