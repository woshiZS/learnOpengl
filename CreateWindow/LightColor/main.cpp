#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <algorithm>
#include <memory>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Camera.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;
Camera camera;
float lastFrame = 0.0f;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double x, double y);
unsigned int loadTexture(char const* path);

template<typename ... Args>
std::string string_format(const std::string& format, Args ... args)
{
	int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
	if (size_s <= 0) { throw std::runtime_error("Error during formatting."); }
	auto size = static_cast<size_t>(size_s);
	std::unique_ptr<char[]> buf(new char[size]);
	std::snprintf(buf.get(), size, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

int main() {
	//test();
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, WIDTH, HEIGHT);
	
	Shader* cubeShader = new Shader("cube.vert", "cube.frag");
	Shader* lightShader = new Shader("light.vert", "light.frag");
	glEnable(GL_DEPTH_TEST); 

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // capture and hide the cursor

	float cubeVertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
	};

	unsigned int VAO, lightVAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW); 

	glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	glEnableVertexAttribArray(6);
	glEnableVertexAttribArray(7);
	glEnableVertexAttribArray(8);

	unsigned int diffuseMap = loadTexture("box.png");
	unsigned int specularMap = loadTexture("specular.png");
	unsigned int emissionMap = loadTexture("matrix.jpg");

	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(6);

	cubeShader->use();
	
	cubeShader->setVec3("material.specular", 0.5f, 0.5f, 0.5f);
	cubeShader->setFloat("material.shininess", 16.0f);
	cubeShader->setInt("material.diffuse", 0);
	cubeShader->setInt("material.specular", 1);
	cubeShader->setInt("material.emission", 2);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularMap);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, emissionMap);

	

	float radius = 3.0f;
	// we need to translate it to the 
	lastFrame = glfwGetTime();
	while (!glfwWindowShouldClose(window)) {
		float timeVal = (float)glfwGetTime();
		processInput(window);
		// rendering commands here.
		glClearColor(0.2f, 0.3f, 0.3f, 0.1f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		float deltaTime = timeVal - lastFrame;
		lastFrame = timeVal;
		camera.UpdatePosition(window, deltaTime);

		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = camera.GetProjectionMatrix();
		glm::vec3 lightColor(1.0f);
		/*lightColor.x = sin(timeVal * 2.0f);
		lightColor.y = sin(timeVal * 0.7f);
		lightColor.z = sin(timeVal * 1.3f);*/

		glm::vec3 diffuseColor = lightColor * 0.5f;
		glm::vec3 ambientColor = lightColor * 0.2f;

		lightShader->use();
		lightShader->setMat4("view", glm::value_ptr(view));
		lightShader->setMat4("projection", glm::value_ptr(projection));
		lightShader->setVec3("lightColor", glm::value_ptr(lightColor));
		for (int i = 0; i < 4; i++) {
			glm::mat4 lightModelMatrix(1.0f);
			lightModelMatrix = glm::translate(lightModelMatrix, pointLightPositions[i]);
			lightModelMatrix = glm::scale(lightModelMatrix, glm::vec3(0.2f));
			lightShader->setMat4("model", glm::value_ptr(lightModelMatrix));
			glBindVertexArray(lightVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		cubeShader->use();
		cubeShader->setMat4("view", glm::value_ptr(view));
		cubeShader->setMat4("projection", glm::value_ptr(projection));
		cubeShader->setVec3("viewPos", glm::value_ptr(camera.GetCameraPos()));
		
#pragma region LightingUniform
		cubeShader->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		cubeShader->setVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
		cubeShader->setVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
		cubeShader->setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);

		for (int i = 0; i < 4; ++i) {
			// need construct format string 
			std::string nameTemp = string_format("pointLights[%d].", i);
			cubeShader->setVec3(nameTemp + "position", glm::value_ptr(pointLightPositions[i]));
			cubeShader->setFloat(nameTemp + "contant", 1.0f);
			cubeShader->setFloat(nameTemp + "linear", 0.09f);
			cubeShader->setFloat(nameTemp + "quadratic", 0.032f);

			cubeShader->setVec3(nameTemp + "ambient", 0.2f, 0.2f, 0.2f);
			cubeShader->setVec3(nameTemp + "diffuse", 0.5f, 0.5f, 0.5f);
			cubeShader->setVec3(nameTemp + "specular", 1.0f, 1.0f, 1.0f);
		}

		cubeShader->setVec3("spotLight.position", glm::value_ptr(camera.GetCameraPos()));
		cubeShader->setVec3("spotLight.spotDir", glm::value_ptr(camera.GetCameraFront()));
		cubeShader->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		cubeShader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));

		cubeShader->setVec3("spotLight.ambient", 0.2f, 0.2f, 0.2f);
		cubeShader->setVec3("spotLight.diffuse", 0.5f, 0.5f, 0.5f);
		cubeShader->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);

#pragma endregion


		//cubeShader->setFloat("light.constant", 1.0f);
		//cubeShader->setFloat("light.linear", 0.09f);
		//cubeShader->setFloat("light.quadratic", 0.032f);

		//cubeShader->setVec3("light.position", glm::value_ptr(camera.GetCameraPos()));
		//cubeShader->setVec3("light.direction", glm::value_ptr(camera.GetCameraFront()));
		//cubeShader->setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
		//cubeShader->setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
		////cubeShader->setVec3("light.direction", -0.2f, -1.0f, -0.3f);

		//cubeShader->setVec3("light.ambient", glm::value_ptr(ambientColor));
		//cubeShader->setVec3("light.diffuse", glm::value_ptr(diffuseColor));
		//cubeShader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		/*cubeShader->setFloat("deltaY", timeVal);
		cubeShader->setFloat("MatrixLight", 2 * abs(sin(timeVal)));*/



		glBindVertexArray(VAO);
		cubeShader->use();
		for (unsigned int i = 0; i < 10; ++i) {
			glm::mat4 model(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			cubeShader->setMat4("model", glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	camera.UpdateAngle(xpos, ypos);
}

void scrollCallback(GLFWwindow* window, double x, double y) {
	camera.updateZoom(x, y);
}

unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
