#include "Shader.h"


Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
{
	std::ifstream vertexFile;
	std::ifstream fragmentFile;
	std::ifstream geometryFile;
	std::stringstream vertexSStream;
	std::stringstream fragmentSStream;
	std::stringstream geometrySStream;

	vertexFile.open(vertexPath);
	fragmentFile.open(fragmentPath);
	if (geometryPath)
		geometryFile.open(geometryPath);

	vertexFile.exceptions(std::ifstream::failbit || std::ifstream::badbit);
	fragmentFile.exceptions(std::ifstream::failbit || std::ifstream::badbit);
	if (geometryPath)
		geometryFile.exceptions(std::ifstream::failbit || std::ifstream::badbit);

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

		if (geometryPath && geometryFile.is_open() == false)
		{
			throw std::exception("open geometry shader file error");
		}


		vertexSStream << vertexFile.rdbuf();
		fragmentSStream << fragmentFile.rdbuf();
		if (geometryPath)
			geometrySStream << geometryFile.rdbuf();

		vertexFile.close();
		fragmentFile.close();
		if (geometryFile.is_open())
			geometryFile.close();

		vertexString = vertexSStream.str();
		fragmentString = fragmentSStream.str();
		geometryString = geometrySStream.str();

		vertexSource = vertexString.c_str();
		fragmentSource = fragmentString.c_str();
		geometrySource = geometryString.c_str();

		unsigned int vertex, fragment, geometry;
		bool hasGeo = false;

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertexSource, nullptr);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragmentSource, nullptr);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");

		if (geometryPath)
		{
			geometry = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometry, 1, &geometrySource, nullptr);
			glCompileShader(geometry);
			checkCompileErrors(geometry, "GEOMETRY");
			hasGeo = true;
		}

		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		if (hasGeo)
			glAttachShader(ID, geometry);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");

		glDeleteShader(vertex);
		glDeleteShader(fragment);
		if (hasGeo)
			glDeleteShader(geometry);
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
			std::cout << type << " Shader compile error: " << infoLog << std::endl;
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

void Shader::setVec3(const std::string& name, const float *f) const {
	glUniform3f(glGetUniformLocation(ID, name.c_str()), f[0], f[1], f[2]);
}

void Shader::setMat4(const std::string& name, float* matrixDataStream) const {
	int modelLoc = glGetUniformLocation(ID, name.c_str());
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, matrixDataStream);
}