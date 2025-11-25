#include "../headers/GrassBlock.hpp"

GrassBlock::GrassBlock() {}

const std::string &GrassBlock::GetName() const { return name; }

unsigned int GrassBlock::GetTextureId() const { return textureId; }
