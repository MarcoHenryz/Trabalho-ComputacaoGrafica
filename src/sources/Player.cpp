#include "../headers/Player.hpp"
#include <GLFW/glfw3.h>
#include <limits>

Player::Player(unsigned int screenWidth, unsigned int screenHeight)
    : lastX(static_cast<float>(screenWidth) / 2.0f),
      lastY(static_cast<float>(screenHeight) / 2.0f), firstMouse(true),
      deltaTime(0.0f), lastFrame(0.0f), camera(glm::vec3(0.0f, 0.0f, 3.0f)) {}

void Player::BindCallbacks(GLFWwindow *window)
{
  // salvo o ponteiro do player na para usar nos callbacks
  glfwSetWindowUserPointer(window, this);
  glfwSetCursorPosCallback(window, MouseCallback);
  glfwSetScrollCallback(window, ScrollCallback);
}

void Player::UpdateFrameTime()
{
  // marca quanto tempo passou desde o frame anterior
  float currentFrame = glfwGetTime();
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;
}

void Player::ProcessInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  // movimentação básica WASD com deltaTime para ficar suave
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(RIGHT, deltaTime);
}

const Camera &Player::GetCamera() const { return camera; }

Camera &Player::AccessCamera() { return camera; }

void Player::HandleMouseMove(double xpos, double ypos)
{
  // primeira vez que o mouse mexe, só posiciono o lastX/lastY pra evitar pulo
  if (firstMouse)
  {
    lastX = static_cast<float>(xpos);
    lastY = static_cast<float>(ypos);
    firstMouse = false;
  }

  float xoffset = static_cast<float>(xpos) - lastX;
  float yoffset = lastY - static_cast<float>(ypos);

  lastX = static_cast<float>(xpos);
  lastY = static_cast<float>(ypos);

  camera.ProcessMouseMovement(xoffset, yoffset);
}

void Player::HandleScroll(double yoffset)
{
  camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

bool Player::PickBlock(const std::vector<BlockCollection> &blocks,
                       glm::vec3 &hitPos, size_t &hitIndex,
                       float maxDistance) const
{
  hitIndex = static_cast<size_t>(-1);
  float closestT = std::numeric_limits<float>::max();
  glm::vec3 rayDir = glm::normalize(camera.Front);

  for (size_t i = 0; i < blocks.size(); ++i)
  {
    const auto *posList = blocks[i].positions;
    for (const auto &pos : *posList)
    {
      glm::vec3 toBlock = pos - camera.Position;
      float t = glm::dot(toBlock, rayDir);
      if (t < 0.0f || t > maxDistance)
        continue;
      // distância do bloco até o raio
      glm::vec3 closest = camera.Position + rayDir * t;
      float dist = glm::length(closest - pos);
      if (dist < 0.6f && t < closestT)
      {
        closestT = t;
        hitIndex = i;
        hitPos = pos;
      }
    }
  }

  return hitIndex != static_cast<size_t>(-1);
}

bool Player::RemoveBlockAt(std::vector<glm::vec3> &positions,
                           const glm::vec3 &worldPos)
{
  for (auto it = positions.begin(); it != positions.end(); ++it)
  {
    if (glm::all(glm::lessThan(glm::abs(*it - worldPos),
                               glm::vec3(0.001f))))
    {
      positions.erase(it);
      return true;
    }
  }
  return false;
}

void Player::MouseCallback(GLFWwindow *window, double xpos, double ypos)
{
  auto *player = static_cast<Player *>(glfwGetWindowUserPointer(window));
  if (player)
  {
    player->HandleMouseMove(xpos, ypos);
  }
}

void Player::ScrollCallback(GLFWwindow *window, double xoffset,
                            double yoffset)
{
  (void)xoffset;
  auto *player = static_cast<Player *>(glfwGetWindowUserPointer(window));
  if (player)
  {
    player->HandleScroll(yoffset);
  }
}
