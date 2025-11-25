#ifndef DIRT_BLOCK_HPP
#define DIRT_BLOCK_HPP

#include "Block.hpp"
#include <string>

class DirtBlock : public Block {
public:
  explicit DirtBlock(unsigned int texture);
  const std::string &GetName() const override;
  unsigned int GetTextureId() const override;

private:
  std::string name = "dirt";
};

#endif
