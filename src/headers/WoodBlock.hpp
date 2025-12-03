#ifndef WOOD_BLOCK_HPP
#define WOOD_BLOCK_HPP

#include "Block.hpp"
#include <string>

class WoodBlock : public Block {
public:
  explicit WoodBlock(unsigned int texture);
  const std::string &GetName() const override;
  unsigned int GetTextureId() const override;
  const std::array<int, 6> &GetFaceTileIndices() const override;

private:
  std::string name = "wood";
};

#endif
