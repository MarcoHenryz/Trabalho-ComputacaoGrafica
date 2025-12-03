#include "../headers/DirtBlock.hpp"

DirtBlock::DirtBlock(unsigned int texture) {
  textureId = texture;
  // tudo tile 2 = terra
  faceTiles = {2, 2, 2, 2, 2, 2};
}

const std::string &DirtBlock::GetName() const { return name; }

unsigned int DirtBlock::GetTextureId() const { return textureId; }

const std::array<int, 6> &DirtBlock::GetFaceTileIndices() const {
  return faceTiles;
}
