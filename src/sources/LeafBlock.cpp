#include "../headers/LeafBlock.hpp"

LeafBlock::LeafBlock(unsigned int texture) {
  textureId = texture;
  // tudo 7 = folhas
  faceTiles = {7, 7, 7, 7, 7, 7};
}

const std::string &LeafBlock::GetName() const { return name; }

unsigned int LeafBlock::GetTextureId() const { return textureId; }

const std::array<int, 6> &LeafBlock::GetFaceTileIndices() const {
  return faceTiles;
}
