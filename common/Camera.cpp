#include "Camera.h"

glm::mat4 Camera::GetViewMatrix() {
	return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

glm::mat4 Camera::GetProjectionMatrix() {
	return glm::perspective(glm::radians(fov), widthHeightRatio, near, far);
}