#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in float aFaceId; // qual face (0-5) este vértice representa

out vec2 AtlasUV;
out vec3 FragPos;
out vec3 Normal;
out vec4 FragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec2 faceOffsets[6];  // canto inferior esquerdo do tile da face
uniform vec2 faceScales[6];   // largura/altura em UV do tile da face
uniform mat4 lightSpaceMatrix;
uniform float time;
uniform int leafBlock;

const vec3 FACE_NORMALS[6] = vec3[6](
    vec3(0.0, 0.0, -1.0), // back
    vec3(0.0, 0.0, 1.0),  // front
    vec3(-1.0, 0.0, 0.0), // left
    vec3(1.0, 0.0, 0.0),  // right
    vec3(0.0, -1.0, 0.0), // bottom
    vec3(0.0, 1.0, 0.0)   // top
);

void main()
{
    vec3 pos = aPos;
    if (leafBlock == 1) {
        // balanço mais visível das folhas, vento fixo pra esquerda-frente
        vec3 windDir = normalize(vec3(0.7, 0.0, 0.4));
        float sway = sin(time * 2.0 + aPos.x + aPos.z) * 0.05;
        pos += windDir * sway;
    }

    vec4 worldPos = model * vec4(pos, 1.0);
    gl_Position = projection * view * worldPos;

    int faceIndex = int(aFaceId + 0.5);
    // UV final = canto do tile + coord local * tamanho do tile
    AtlasUV = faceOffsets[faceIndex] + aTexCoord * faceScales[faceIndex];

    FragPos = worldPos.xyz;
    // normal da face transformada pro mundo
    mat3 normalMatrix = mat3(transpose(inverse(model)));
    Normal = normalize(normalMatrix * FACE_NORMALS[faceIndex]);
    // posição do fragmento no espaço da luz (pra testar sombra)
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
}
