#include "../headers/GrassBlock.hpp"

GrassBlock::GrassBlock(unsigned int texture)
{
  textureId = texture;
  // trás, frente, esquerda, direita, baixo, cima
  // 0 = cima grama, 1 = lado grama, 2 = terra
  faceTiles = {1, 1, 1, 1, 2, 0};
}

const std::string &GrassBlock::GetName() const { return name; }

unsigned int GrassBlock::GetTextureId() const { return textureId; }

const std::array<int, 6> &GrassBlock::GetFaceTileIndices() const
{
  return faceTiles;
}
