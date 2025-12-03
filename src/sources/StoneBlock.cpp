#include "../headers/StoneBlock.hpp"

StoneBlock::StoneBlock(unsigned int texture) {
  textureId = texture;
  // tudo tile 3 = pedra lisa
  faceTiles = {3, 3, 3, 3, 3, 3};
}

const std::string &StoneBlock::GetName() const { return name; }

unsigned int StoneBlock::GetTextureId() const { return textureId; }

const std::array<int, 6> &StoneBlock::GetFaceTileIndices() const {
  return faceTiles;
}
