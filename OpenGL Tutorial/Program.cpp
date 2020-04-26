#include "Program.h"

#include <fstream>
#include <iostream>

void Program::bind()
{
	glUseProgram(progID);
}

void Program::unbind()
{
	glUseProgram(0);
}

bool Program::init(const char *vert, const char *frag)
{
	shaderID.insert(std::pair<GLuint, GLuint>
		(GL_VERTEX_SHADER, glCreateShader(GL_VERTEX_SHADER)));
	shaderID.insert(std::pair<GLuint, GLuint>
		(GL_FRAGMENT_SHADER, glCreateShader(GL_FRAGMENT_SHADER)));

	progID = glCreateProgram();

	if (!loadShader(vert, GL_VERTEX_SHADER))
	{
		return false;
	}
	
	if (!loadShader(frag, GL_FRAGMENT_SHADER))
	{
		return false;
	}

	if (!compile())
	{
		return false;
	}

	return true;
}

bool Program::compile()
{
	int success;
	char infoLog[512];

	const char *vertexShaderSource = shaderText[GL_VERTEX_SHADER].c_str();
	const char *fragmentShaderSource = shaderText[GL_FRAGMENT_SHADER].c_str();

	glShaderSource(shaderID[GL_VERTEX_SHADER], 1,
		&vertexShaderSource, NULL);
	glCompileShader(shaderID[GL_VERTEX_SHADER]);

	glShaderSource(shaderID[GL_FRAGMENT_SHADER], 1,
		&fragmentShaderSource, NULL);
	glCompileShader(shaderID[GL_FRAGMENT_SHADER]);

	glGetShaderiv(shaderID[GL_VERTEX_SHADER], GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderID[GL_VERTEX_SHADER], 512,
			NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
			<< infoLog << std::endl;
		return false;
	}

	glGetShaderiv(shaderID[GL_FRAGMENT_SHADER], GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderID[GL_FRAGMENT_SHADER], 512,
			NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
			<< infoLog << std::endl;
		return false;
	}

	glAttachShader(progID, shaderID[GL_VERTEX_SHADER]);
	glAttachShader(progID, shaderID[GL_FRAGMENT_SHADER]);
	glLinkProgram(progID);

	glGetProgramiv(progID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(progID, 512, NULL, infoLog);
		std::cout << "ERROR:SHADER_PROGRAM::LINK_FAILED\n"
			<< infoLog << std::endl;
		return false;
	}

	glDeleteShader(shaderID[GL_VERTEX_SHADER]);
	glDeleteShader(shaderID[GL_FRAGMENT_SHADER]);

	return true;
}

bool Program::loadShader(const char *src, GLuint shaderType)
{
	std::ifstream in;
	std::string text, line;

	in.open(src);

	if (in.is_open())
	{
		while (std::getline(in, line))
		{
			text += line + "\n";
		}
	}
	else
	{
		return false;
	}

	shaderText.insert(std::pair<GLuint, const std::string>(shaderType, text));

	in.close();
	return true;
}

void Program::setBool(const std::string *name, bool value) const
{
	glUniform1i(glGetUniformLocation(progID, name->c_str()), (int)value);
}

void Program::setInt(const std::string *name, int value) const
{
	glUniform1i(glGetUniformLocation(progID, name->c_str()), value);
}

void Program::setFloat(const std::string *name, float value) const
{
	glUniform1f(glGetUniformLocation(progID, name->c_str()), value);
}

const char *Program::getShader(GLuint shaderType)
{
	return shaderText[shaderType].c_str();
}