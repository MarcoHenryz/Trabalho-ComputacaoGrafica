#ifndef STONE_BLOCK_HPP
#define STONE_BLOCK_HPP

#include "Block.hpp"
#include <string>

class StoneBlock : public Block {
public:
  explicit StoneBlock(unsigned int texture);
  const std::string &GetName() const override;
  unsigned int GetTextureId() const override;
  const std::array<int, 6> &GetFaceTileIndices() const override;

private:
  std::string name = "stone";
};

#endif
