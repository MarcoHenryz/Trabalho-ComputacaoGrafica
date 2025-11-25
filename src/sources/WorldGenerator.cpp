#include "../headers/WorldGenerator.hpp"

std::vector<glm::vec3> WorldGenerator::GenerateFlatChunk() const {
  std::vector<glm::vec3> cubePositions;
  cubePositions.reserve(16 * 16);
  for (int i = 0; i < 16; i++)
    for (int j = 0; j < 16; j++)
      cubePositions.push_back(glm::vec3(-8.0 + i, -2.0f, -8.0 + j));
  return cubePositions;
}
