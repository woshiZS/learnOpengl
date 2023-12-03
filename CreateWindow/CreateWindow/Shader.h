#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	void use();
	unsigned int getShaderProgramID();
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	//~Shader();
private:
	std::string vertexString;
	std::string fragmentString;
	const char* vertexSource;
	const char* fragmentSource;

	unsigned int ID; // Shader Program ID

	void checkCompileErrors(GLuint shader, std::string type);
};