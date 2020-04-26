#pragma once

#include <map>
#include <string>

#include <glad/glad.h>

class Program
{
public:
	void bind();
	void unbind();
	bool init(const char *vert, const char *frag);
	const char *getShader(GLuint shaderType);

	GLuint progID;

private:
	bool compile();
	bool loadShader(const char *src, GLuint shaderType);

	std::map<GLuint, const std::string> shaderText;
	std::map<GLuint, GLuint> shaderID;
};