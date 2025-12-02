#include "../headers/WorldGenerator.hpp"

WorldGenerator::WorldIslands WorldGenerator::GenerateTwoIslands() const {
  WorldIslands islands;

  auto addFlatIsland = [](std::vector<glm::vec3> &target, const glm::vec3 &base,
                          int size) {
    for (int x = 0; x < size; ++x)
      for (int z = 0; z < size; ++z)
        target.emplace_back(base.x + static_cast<float>(x), base.y,
                            base.z + static_cast<float>(z));
  };

  // exemplo de grama
  addFlatIsland(islands.grassBlocks, glm::vec3(-4.0f, -2.0f, -4.0f), 4);

  // exemplo de terra
  addFlatIsland(islands.dirtBlocks, glm::vec3(3.0f, -2.0f, 3.0f), 3);

  return islands;
}
