#version 330 core
out vec4 FragColor;

in vec2 AtlasUV;
in vec3 FragPos;
in vec3 Normal;
in vec4 FragPosLightSpace;

uniform sampler2D block_texture;
uniform vec3 lightDir;
uniform vec3 lightColor;
uniform float ambientStrength;
uniform sampler2D shadowMap;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // converte de clip space da luz para UV 0..1
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.z > 1.0)
        return 0.0;

    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float bias = 0.0015; // evita acne da sombra
    return currentDepth - bias > closestDepth ? 1.0 : 0.0;
}

void main()
{
    vec4 texColor = texture(block_texture, AtlasUV);
    if (texColor.a < 0.05)
        discard; // ignora pixels totalmente transparentes

    vec3 norm = normalize(Normal);
    vec3 lightDirN = normalize(-lightDir);

    float diff = max(dot(norm, lightDirN), 0.0);

    vec3 ambient = ambientStrength * lightColor;
    vec3 diffuse = diff * lightColor;

    float shadow = ShadowCalculation(FragPosLightSpace);
    vec3 lighting = ambient + (1.0 - shadow) * diffuse;
    vec3 result = texColor.rgb * lighting;

    FragColor = vec4(result, texColor.a);
}
