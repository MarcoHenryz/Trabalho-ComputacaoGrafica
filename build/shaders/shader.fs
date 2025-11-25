#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D block_texture;

void main()
{
	FragColor = texture(block_texture, TexCoord);
}
