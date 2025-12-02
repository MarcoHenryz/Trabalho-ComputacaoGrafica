#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Block.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "glm/glm.hpp"
#include <GLFW/glfw3.h>
#include <vector>

class Renderer {
public:
  Renderer();
  ~Renderer();

  void Initialize();
  void Render(const std::vector<glm::vec3> &cubePositions, const Block &block,
              Shader &shader, const Camera &camera,
              unsigned int screenWidth, unsigned int screenHeight);
  void Cleanup();

  static void FramebufferSizeCallback(GLFWwindow *window, int width,
                                      int height);

private:
  void SetupBuffers();
  unsigned int VBO;
  unsigned int VAO;
};

#endif
