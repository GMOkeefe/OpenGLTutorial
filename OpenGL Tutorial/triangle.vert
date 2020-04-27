#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
layout (location = 2) in vec2 aTex;

out vec3 col;
out vec2 tex;

void main()
{
	col = aCol;
	tex = aTex;
	gl_Position = vec4(aPos, 1.0);
}