#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Camera.hpp"
#include <GLFW/glfw3.h>

class Player {
public:
  Player(unsigned int screenWidth, unsigned int screenHeight);

  void BindCallbacks(GLFWwindow *window);
  void UpdateFrameTime();
  void ProcessInput(GLFWwindow *window);

  const Camera &GetCamera() const;
  Camera &AccessCamera();

private:
  void HandleMouseMove(double xpos, double ypos);
  void HandleScroll(double yoffset);

  static void MouseCallback(GLFWwindow *window, double xpos, double ypos);
  static void ScrollCallback(GLFWwindow *window, double xoffset,
                             double yoffset);

  float lastX;
  float lastY;
  bool firstMouse;
  float deltaTime;
  float lastFrame;
  Camera camera;
  // TODO: plug survival movement logic here (stamina, hunger, physics tuning)
};

#endif
