#include "../headers/Renderer.hpp"
#include "../headers/glad/glad.h"
#include "../headers/glm/gtc/matrix_transform.hpp"
#include <string>

namespace {
// Atlas 80x32, cada face 16x16
// grama cima   - grama lado   - terra  - pedra lisa - nada
// madeira lado - madeira cima - folhas - carvão     - nada
constexpr float ATLAS_W = 80.0f;
constexpr float ATLAS_H = 32.0f;
constexpr float TILE_W_PX = 16.0f;
constexpr float TILE_H_PX = 16.0f;

struct RegionUV {
  glm::vec2 min;
  glm::vec2 max;
};

RegionUV RegionForTile(int tileIndex) {
  const int col = tileIndex % 5;
  const int row = tileIndex / 5;
  const float left = col * TILE_W_PX;
  const float top = row * TILE_H_PX;
  const float right = left + TILE_W_PX;
  const float bottom = top + TILE_H_PX;

  const float u0 = left / ATLAS_W;
  const float u1 = right / ATLAS_W;
  const float v0 = 1.0f - (bottom / ATLAS_H); // origem em cima
  const float v1 = 1.0f - (top / ATLAS_H);

  return RegionUV{glm::vec2(u0, v0), glm::vec2(u1, v1)};
}

// trás, frente, esquerda, direita, baixo, cima
const float VERTICES[] = {
    // trás (z -0.5) face 0
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,

    // frente (z +0.5) face 1
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,

    // esquerda (x -0.5) face 2
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 2.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 2.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 2.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 2.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 2.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 2.0f,

    // direita (x +0.5) face 3
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 3.0f,
     0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 3.0f,
     0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 3.0f,
     0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 3.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 3.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 3.0f,

    // baixo (y -0.5) face 4
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 4.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 4.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 4.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 4.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 4.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 4.0f,

    // cima (y +0.5) face 5
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 5.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 5.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 5.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 5.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 5.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 5.0f};
} // namespace

Renderer::Renderer() : VBO(0), VAO(0) {}

Renderer::~Renderer() { Cleanup(); }

void Renderer::Initialize() { SetupBuffers(); }

void Renderer::Render(const std::vector<glm::vec3> &cubePositions,
                      const Block &block, Shader &shader, const Camera &camera,
                      unsigned int screenWidth, unsigned int screenHeight) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, block.GetTextureId());

  shader.use();

  // Aqui escolho a fatia do atlas para cada face, dependendo do bloco.
  const auto &faces = block.GetFaceTileIndices();
  for (size_t i = 0; i < faces.size(); ++i) {
    const auto uv = RegionForTile(faces[i]);
    shader.setVec2("faceOffsets[" + std::to_string(i) + "]", uv.min);
    shader.setVec2("faceScales[" + std::to_string(i) + "]",
                   uv.max - uv.min);
  }

  glm::mat4 projection = glm::perspective(
      glm::radians(camera.Zoom),
      static_cast<float>(screenWidth) / static_cast<float>(screenHeight), 0.1f,
      100.0f);
  shader.setMat4("projection", projection);

  glm::mat4 view = camera.GetViewMatrix();
  shader.setMat4("view", view);

  glBindVertexArray(VAO);
  for (const auto &cubePosition : cubePositions) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, cubePosition);
    shader.setMat4("model", model);

    glDrawArrays(GL_TRIANGLES, 0, 36);
  }
}

void Renderer::Cleanup() {
  if (VAO != 0) {
    glDeleteVertexArrays(1, &VAO);
    VAO = 0;
  }
  if (VBO != 0) {
    glDeleteBuffers(1, &VBO);
    VBO = 0;
  }
}

void Renderer::FramebufferSizeCallback(GLFWwindow *window, int width,
                                       int height) {
  (void)window;
  glViewport(0, 0, width, height);
}

void Renderer::SetupBuffers() {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);

  // posição
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)0);
  glEnableVertexAttribArray(0);

  // coordenada normalizada
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // id da face (0-5) para buscar no atlas
  glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(5 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
}
