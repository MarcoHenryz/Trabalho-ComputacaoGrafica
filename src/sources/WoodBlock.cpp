#include "../headers/WoodBlock.hpp"

WoodBlock::WoodBlock(unsigned int texture) {
  textureId = texture;
  // trás, frente, esquerda, direita, baixo, cima
  // 5 = lado tronco, 6 = topo tronco
  faceTiles = {5, 5, 5, 5, 6, 6};
}

const std::string &WoodBlock::GetName() const { return name; }

unsigned int WoodBlock::GetTextureId() const { return textureId; }

const std::array<int, 6> &WoodBlock::GetFaceTileIndices() const {
  return faceTiles;
}
