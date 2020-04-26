#version 330 core
out vec4 FragColor;
in vec3 color;

uniform float trans;

void main()
{
	FragColor = vec4(color, trans);
}