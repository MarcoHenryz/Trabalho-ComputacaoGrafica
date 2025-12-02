#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in float aFaceId; // qual face (0-5) este vértice representa

out vec2 AtlasUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec2 faceOffsets[6];  // canto inferior esquerdo do tile da face
uniform vec2 faceScales[6];   // largura/altura em UV do tile da face

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    int faceIndex = int(aFaceId + 0.5);
    // UV final = canto do tile + coord local * tamanho do tile
    AtlasUV = faceOffsets[faceIndex] + aTexCoord * faceScales[faceIndex];
}
