#include "../headers/DirtBlock.hpp"

DirtBlock::DirtBlock(unsigned int texture) { textureId = texture; }

const std::string &DirtBlock::GetName() const { return name; }

unsigned int DirtBlock::GetTextureId() const { return textureId; }
