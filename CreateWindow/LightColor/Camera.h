#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

class Camera
{
private:
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	glm::vec3 cameraRight;
	float yaw = -90.0f, pitch = 0.0f;
	float lastX = 400, lastY = 300;
	float widthHeightRatio = 800.0f/ 600.0f;
	float fov = 45.0f, near = 0.1f, far = 100.0f;
	float cameraMoveSpeed;
	const float sensitivity = 0.05f;
public:
	Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)):
	cameraPos(pos), cameraFront(front), cameraUp(up)
	{
	}
	// 实际上glm的look at矩阵只需要位置，视线上某一个点的位置，以及向上位置的朝向
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();

	void UpdateFront() {
		glm::vec3 direction;
		direction.y = sin(glm::radians(pitch));
		direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		direction.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
		cameraFront = direction;
	}


	// This one is called in every frame
	void UpdatePosition(GLFWwindow* window, float deltaTime) {
		float cameraSpeed = 0.02f * deltaTime;
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

	// The following are used by callbacks

	void UpdateAngle(double xpos, double ypos) {
		float xoffset = xpos - lastX;
		float yoffset = ypos - lastY;
		lastX = xpos;
		lastY = ypos;

		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw -= xoffset;
		pitch += yoffset;
		pitch = std::max(-89.0f, std::min(89.0f, pitch));
		UpdateFront();
	}

	void updateZoom(double xoffset, double yoffset) {
		fov -= (float)yoffset;
		fov = std::max(1.0f, std::min(45.0f, fov));
	}
};

