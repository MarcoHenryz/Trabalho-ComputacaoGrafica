#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Block.hpp"
#include "Camera.hpp"
#include <GLFW/glfw3.h>
#include <vector>

struct BlockCollection {
  std::vector<glm::vec3> *positions;
  Block *block;
};

class Player {
public:
  Player(unsigned int screenWidth, unsigned int screenHeight);

  void BindCallbacks(GLFWwindow *window);
  void UpdateFrameTime();
  void ProcessInput(GLFWwindow *window);
  // raycast simples para achar bloco na mira
  bool PickBlock(const std::vector<BlockCollection> &blocks, glm::vec3 &hitPos,
                 size_t &hitIndex, float maxDistance = 12.0f) const;
  static bool RemoveBlockAt(std::vector<glm::vec3> &positions,
                            const glm::vec3 &worldPos);

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
