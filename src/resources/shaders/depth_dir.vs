#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 lightSpaceMatrix;
uniform float time;
uniform int leafBlock;

void main()
{
    vec3 pos = aPos;
    if (leafBlock == 1) {
        // balanço mais visível das folhas com seno no eixo xz
        vec3 windDir = normalize(vec3(0.7, 0.0, 0.4));
        float sway = sin(time * 2.0 + aPos.x + aPos.z) * 0.05;
        pos += windDir * sway;
    }
    gl_Position = lightSpaceMatrix * model * vec4(pos, 1.0);
}
