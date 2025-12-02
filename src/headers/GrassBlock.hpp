#ifndef GRASS_BLOCK_HPP
#define GRASS_BLOCK_HPP

#include "Block.hpp"
#include <string>

class GrassBlock : public Block {
public:
  explicit GrassBlock(unsigned int texture);
  const std::string &GetName() const override;
  unsigned int GetTextureId() const override;
  const std::array<int, 6> &GetFaceTileIndices() const override;

private:
  std::string name = "grass";
};

#endif
