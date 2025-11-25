#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <string>

class Block {
public:
  virtual ~Block() = default;
  virtual const std::string &GetName() const = 0;
  virtual unsigned int GetTextureId() const = 0;

protected:
  unsigned int textureId = 0;
};

#endif
