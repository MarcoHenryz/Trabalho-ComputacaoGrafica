#include "../headers/TextureLoader.hpp"
#include "../headers/glad/glad.h"
#include "../headers/stb_image.h"
#include <iostream>
#include <string>
#include <array>

unsigned int TextureLoader::LoadAtlasTexture() const
{
  unsigned int atlasTexture = 0;

  glGenTextures(1, &atlasTexture);
  glBindTexture(GL_TEXTURE_2D, atlasTexture);

  // filtros simples (pixel art) e wrap padrão
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  int width = 0;
  int height = 0;
  int channels = 0;

  // origem no canto inferior esquerdo
  stbi_set_flip_vertically_on_load(true);

  // carrega atlas
  std::string path = "src/resources/textures/atlas_texture.png";

  unsigned char *data = nullptr;
  std::string loadedPath;
  data = stbi_load(path.c_str(), &width, &height, &channels, 0);
  if (data)
  {
    loadedPath = path;
  }

  if (!data)
  {
    std::cout << "Erro ao carregar textura! "
              << path << "\n"
              << ")\nMotivo: " << stbi_failure_reason() << std::endl;
  }
  else
  {
    GLenum format = channels == 4 ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    std::cout << "Textura carregada\n";
  }

  stbi_image_free(data);

  return atlasTexture;
}
