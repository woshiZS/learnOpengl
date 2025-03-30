#include "Shader.h"


Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::ifstream vertexFile;
	std::ifstream fragmentFile;
	std::stringstream vertexSStream;
	std::stringstream fragmentSStream;

	vertexFile.open(vertexPath);
	fragmentFile.open(fragmentPath);

	vertexFile.exceptions(std::ifstream::failbit || std::ifstream::badbit);
	fragmentFile.exceptions(std::ifstream::failbit || std::ifstream::badbit);

	try
	{
		if(vertexFile.is_open() == false)
		{
			throw std::exception("open vertex shader file error");
		}

		if (fragmentFile.is_open() == false)
		{
			throw std::exception("open fragment shader file error");
		}

		vertexSStream << vertexFile.rdbuf();
		fragmentSStream << fragmentFile.rdbuf();

		vertexFile.close();
		fragmentFile.close();

		vertexString = vertexSStream.str();
		fragmentString = fragmentSStream.str();

		vertexSource = vertexString.c_str();
		fragmentSource = fragmentString.c_str();

		/*printf(vertexSource);
		printf(fragmentSource);*/

		unsigned int vertex, fragment;
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertexSource, nullptr);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragmentSource, nullptr);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");

		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");

		glDeleteShader(vertex);
		glDeleteShader(fragment);

	}
	catch (const std::exception& e)
	{
		printf(e.what());
	}
}

void Shader::use() {
	glUseProgram(ID);
}

unsigned int Shader::getShaderProgramID() {
	return ID;
}

void Shader::checkCompileErrors(GLuint shader, std::string type) {
	int success;
	char infoLog[512];

	if (type == "PROGRAM") {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, sizeof(infoLog), NULL, infoLog);
			std::cout << "Shader linking error: " << infoLog << std::endl;
		}
	}
	else {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
			std::cout << "Shader compile error: " << infoLog << std::endl;
		}
	}
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec3(const std::string& name, float v1, float v2, float v3) const {
	glUniform3f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3);
}