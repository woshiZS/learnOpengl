#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
	void use();
	unsigned int getShaderProgramID();
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec3(const std::string& name, float v1, float v2, float v3) const;
	void setVec3(const std::string& name, const float* f) const;
	void setMat4(const std::string& name, float* matrixDataStream) const;
	//~Shader();
private:
	std::string vertexString;
	std::string fragmentString;
	std::string geometryString;
	const char* vertexSource;
	const char* fragmentSource;
	const char* geometrySource;

	unsigned int ID; // Shader Program ID

	void checkCompileErrors(GLuint shader, std::string type);
};