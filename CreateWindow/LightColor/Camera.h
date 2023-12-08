#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

class Camera
{
private:
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	glm::vec3 cameraRight;
	float yaw = -180.0f, pitch = 0.0f;
	float widthHeightRatio;
	float fov, near, far;
	float cameraMoveSpeed;
public:
	Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)):
	cameraPos(pos), cameraFront(front), cameraUp(up)
	{}
	// 实际上glm的look at矩阵只需要位置，视线上某一个点的位置，以及向上位置的朝向
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();

	void RegisterInput();

	void UpdatePosition(GLFWwindow* window, float deltaTime) {
		float cameraSpeed = 2.5f * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cameraPos += cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cameraPos -= cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cameraPos -= cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cameraPos += cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			cameraPos += cameraSpeed * cameraUp;
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			cameraPos -= cameraSpeed * cameraUp;
	}
};

