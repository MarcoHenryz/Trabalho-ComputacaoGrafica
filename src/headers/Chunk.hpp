#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "Block.hpp"
#include "glm/glm.hpp"
#include <vector>

class Chunk {
public:
  Chunk(const std::vector<glm::vec3> &positions, Block &blockType);
  const std::vector<glm::vec3> &GetBlockPositions() const;
  Block &GetBlockType();

private:
  std::vector<glm::vec3> blockPositions;
  Block *block;
  // TODO: chunk metadata (lighting grid, neighbors, dirty flags)
};

#endif
