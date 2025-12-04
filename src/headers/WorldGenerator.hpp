#ifndef WORLD_GENERATOR_HPP
#define WORLD_GENERATOR_HPP

#include "glm/glm.hpp"
#include <vector>

class WorldGenerator {
public:
  struct CustomMatrix3D {
    std::vector<glm::vec3> grassBlocks;
    std::vector<glm::vec3> dirtBlocks;
    std::vector<glm::vec3> stoneBlocks;
    std::vector<glm::vec3> woodBlocks;
    std::vector<glm::vec3> leafBlocks;
  };

  CustomMatrix3D GenerateCustomMatrix() const;
};

#endif
