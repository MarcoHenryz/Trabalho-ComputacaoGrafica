#include "../headers/WorldGenerator.hpp"
#include <vector>
#include <string>

WorldGenerator::CustomMatrix3D WorldGenerator::GenerateCustomMatrix() const
{
  CustomMatrix3D layout;

  // posição da ilha no mundo
  float originX = -5.0f;
  float originZ = -5.0f;
  float minY    = -5.0f;

  // world[y][z][x]
  // ' ' = vazio
  // P = pedra, T = terra, G = grama, M = madeira, F = folha
  std::vector<std::vector<std::string> > world;

  // pontinha
  world.push_back({
      "           ",
      "     P     ",
      "    PPP    ",
      "   PPPPP   ",
      "    PPP    ",
      "     P     ",
      "           ",
  });

  // pedra aumentando
  world.push_back({
      "     P     ",
      "    PPP    ",
      "   PPPPP   ",
      "  PPPPPPP  ",
      "   PPPPP   ",
      "    PPP    ",
      "     P     ",
  });

  // terra aumentando
  world.push_back({
      "    TTT    ",
      "   TTTTT   ",
      "  TTTTTTT  ",
      " TTTTTTTTT ",
      "  TTTTTTT  ",
      "   TTTTT   ",
      "    TTT    ",
  });

  // grama
  world.push_back({
      "   GGGGG   ",
      "  GGGGGGG  ",
      " GGGGGGGGG ",
      "GGGGGGGGGGG",
      " GGGGGGGGG ",
      "  GGGGGGG  ",
      "   GGGGG   ",
  });

  // tronco
  world.push_back({
      "           ",
      "           ",
      "           ",
      "  M        ",
      "           ",
      "           ",
      "           ",
  });

  // tronco
  world.push_back({
      "           ",
      "           ",
      "           ",
      "  M        ",
      "           ",
      "           ",
      "           ",
  });

  // tronco
  world.push_back({
      "           ",
      "           ",
      "           ",
      "  M        ",
      "           ",
      "           ",
      "           ",
  });

  // folhas maior
  world.push_back({
      "           ",
      "FFFFF      ",
      "FFFFF      ",
      "FFMFF      ",
      "FFFFF      ",
      "FFFFF      ",
      "           ",
  });

  // folhas medio
  world.push_back({
      "           ",
      "           ",
      " FFF       ",
      " FFF       ",
      " FFF       ",
      "           ",
      "           ",
  });


  // folhas topo
  world.push_back({
      "           ",
      "           ",
      "  F        ",
      " FFF       ",
      "  F        ",
      "           ",
      "           ",
  });


  // a partir daqui é só varrer as letras e jogar pro vetor certo
  int height = world.size();
  if (height == 0)
    return layout;

  int depth = world[0].size();       // z
  int width = world[0][0].size();    // x

  for (int iy = 0; iy < height; ++iy)
  {
    float y = minY + iy;

    for (int iz = 0; iz < depth; ++iz)
    {
      const std::string &row = world[iy][iz];

      for (int ix = 0; ix < width; ++ix)
      {
        char c = row[ix];
        if (c == ' ')
          continue;

        float x = originX + ix;
        float z = originZ + iz;

        glm::vec3 pos(x, y, z);

        if (c == 'P')
          layout.stoneBlocks.push_back(pos);
        else if (c == 'T')
          layout.dirtBlocks.push_back(pos);
        else if (c == 'G')
          layout.grassBlocks.push_back(pos);
        else if (c == 'M')
          layout.woodBlocks.push_back(pos);
        else if (c == 'F')
          layout.leafBlocks.push_back(pos);
      }
    }
  }

  return layout;
}
