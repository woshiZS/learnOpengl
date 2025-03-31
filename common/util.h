#pragma once
#include <glad/glad.h>

#include "Camera.h"

unsigned int loadTexture(const char* path);

class GLFWUtil {
 public:
  static GLFWUtil& GetInstance() 
  {
    static GLFWUtil util;
    return util;
  }
  void SetCamera(Camera* camera) { mCamera = camera; }
  const Camera* GetCamera() const noexcept { return mCamera; }
  void framebufferSizeCallback(GLFWwindow* window, int width, int height);
  void mouseCallback(GLFWwindow* window, double xpos, double ypos);
  void scrollCallback(GLFWwindow* window, double x, double y);

 protected:
 private:
  Camera* mCamera{nullptr};
};
