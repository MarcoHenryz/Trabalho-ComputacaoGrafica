#include "../headers/TextureLoader.hpp"
#include "../headers/glad/glad.h"
#include "../headers/stb_image.h"
#include <iostream>

unsigned int TextureLoader::LoadDirtTexture() const {
  unsigned int dirtTexture = 0;

  glGenTextures(1, &dirtTexture);
  glBindTexture(GL_TEXTURE_2D, dirtTexture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);

  unsigned char *data =
      stbi_load("src/resources/textures/dirt.png", &width, &height, &nrChannels,
                0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  return dirtTexture;
}
