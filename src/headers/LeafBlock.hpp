#ifndef LEAF_BLOCK_HPP
#define LEAF_BLOCK_HPP

#include "Block.hpp"
#include <string>

class LeafBlock : public Block {
public:
  explicit LeafBlock(unsigned int texture);
  const std::string &GetName() const override;
  unsigned int GetTextureId() const override;
  const std::array<int, 6> &GetFaceTileIndices() const override;

private:
  std::string name = "leaf";
};

#endif
