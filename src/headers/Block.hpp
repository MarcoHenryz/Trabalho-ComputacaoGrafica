#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <array>
#include <string>

class Block {
public:
  virtual ~Block() = default;
  virtual const std::string &GetName() const = 0;
  virtual unsigned int GetTextureId() const = 0;
  virtual const std::array<int, 6> &GetFaceTileIndices() const = 0;

protected:
  unsigned int textureId = 0;
  // face order: back, front, left, right, bottom, top
  std::array<int, 6> faceTiles{};
};

#endif
