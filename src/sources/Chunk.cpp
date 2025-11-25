#include "../headers/Chunk.hpp"

Chunk::Chunk(const std::vector<glm::vec3> &positions, Block &blockType)
    : blockPositions(positions), block(&blockType) {}

const std::vector<glm::vec3> &Chunk::GetBlockPositions() const {
  return blockPositions;
}

Block &Chunk::GetBlockType() { return *block; }
