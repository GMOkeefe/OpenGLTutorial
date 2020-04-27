#version 330 core
out vec4 FragColor;
in vec3 col;
in vec2 tex;

uniform sampler2D Texture;

void main()
{
	FragColor = texture(Texture, tex);
	FragColor = vec4(col, 1.0);
}