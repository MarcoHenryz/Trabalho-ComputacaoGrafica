#ifndef GRASS_BLOCK_HPP
#define GRASS_BLOCK_HPP

#include "Block.hpp"
#include <string>

class GrassBlock : public Block {
public:
  GrassBlock();
  const std::string &GetName() const override;
  unsigned int GetTextureId() const override;

private:
  std::string name = "grass";
  // TODO: assign grass texture when the asset and binding logic are available
};

#endif
