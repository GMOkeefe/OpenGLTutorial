#version 330 core
out vec4 FragColor;

uniform float trans;

void main()
{
	FragColor = vec4(vec3(1.0), trans);
}