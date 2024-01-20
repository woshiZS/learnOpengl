#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class ComputeShader
{
public:

	ComputeShader(const char* computePath);
	void use();
	unsigned int getShaderProgramID();
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec3(const std::string& name, float v1, float v2, float v3) const;
	void setVec3(const std::string& name, const float* f) const;
	void setMat4(const std::string& name, float* matrixDataStream) const;

private:
	unsigned int ID;
	std::string computeString;
	const char* computeSource;

	void checkCompileErrors(GLuint shader, std::string type);
};

