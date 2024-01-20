#include "ComputeShader.h"

ComputeShader::ComputeShader(const char* computePath)
{
	std::ifstream computeFile;
	std::stringstream computeSStream;
	

	computeFile.open(computePath);

	computeFile.exceptions(std::ifstream::failbit || std::ifstream::badbit);

	try
	{
		if (computeFile.is_open() == false)
		{
			throw std::exception("open compute shader file error");
		}

		computeSStream << computeFile.rdbuf();
	

		computeFile.close();

		computeString = computeSStream.str();

		computeSource = computeString.c_str();

		unsigned int compute;
		compute = glCreateShader(GL_COMPUTE_SHADER);
		glShaderSource(compute, 1, &computeSource, NULL);
		glCompileShader(compute);
		checkCompileErrors(compute, "COMPUTE");
		

		ID = glCreateProgram();
		glAttachShader(ID, compute);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");

		glDeleteShader(compute);

	}
	catch (const std::exception& e)
	{
		printf(e.what());
	}
}

void ComputeShader::checkCompileErrors(GLuint shader, std::string type) {
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


void ComputeShader::use() {
	glUseProgram(ID);
}

unsigned int ComputeShader::getShaderProgramID() {
	return ID;
}

void ComputeShader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void ComputeShader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void ComputeShader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void ComputeShader::setVec3(const std::string& name, float v1, float v2, float v3) const {
	glUniform3f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3);
}

void ComputeShader::setVec3(const std::string& name, const float* f) const {
	glUniform3f(glGetUniformLocation(ID, name.c_str()), f[0], f[1], f[2]);
}

void ComputeShader::setMat4(const std::string& name, float* matrixDataStream) const {
	int modelLoc = glGetUniformLocation(ID, name.c_str());
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, matrixDataStream);
}