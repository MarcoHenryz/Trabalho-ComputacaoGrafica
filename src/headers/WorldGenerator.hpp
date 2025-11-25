#ifndef WORLD_GENERATOR_HPP
#define WORLD_GENERATOR_HPP

#include "glm/glm.hpp"
#include <vector>

class WorldGenerator {
public:
  std::vector<glm::vec3> GenerateFlatChunk() const;
  // TODO: replace with procedural world generation (noise, biomes, structures)
};

#endif
