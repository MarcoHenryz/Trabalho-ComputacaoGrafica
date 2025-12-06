#include "../headers/WorldGenerator.hpp"
#include <string>

WorldGenerator::CustomMatrix3D WorldGenerator::GenerateCustomMatrix() const
{
  CustomMatrix3D layout;

  // formato da ilha
  const char *map[] = {
      "   BBBBB   ",
      "  BBBBBBB  ",
      " BBBBBBBBB ",
      "BBBBBBBBBB ",
      " BBBBBBBBB ",
      "  BBBBBBB  ",
      "   BBBBB   ",
  };

  // centro mais ou menos
  const float originX = -5.0f;
  const float originZ = -5.0f;

  const int rows = static_cast<int>(sizeof(map) / sizeof(map[0]));

  // formato aesthetic de gota
  std::vector<std::string> grassMask;
  grassMask.reserve(rows);
  for (int i = 0; i < rows; ++i)
    grassMask.emplace_back(map[i]);

  auto shrinkMask = [](std::vector<std::string> mask)
  {
    for (size_t i = 0; i < mask.size(); ++i)
    {
      std::string &row = mask[i];
      const auto first = row.find('B');
      const auto last = row.rfind('B');
      if (first != std::string::npos)
        row[first] = ' ';
      if (last != std::string::npos)
        row[last] = ' ';
      if (i == 0 || i + 1 == mask.size())
      {
        for (char &c : row)
          if (c == 'B')
            c = ' ';
      }
    }
    return mask;
  };

  auto applyLayer = [&](const std::vector<std::string> &mask, float y,
                        std::vector<glm::vec3> &target)
  {
    for (int z = 0; z < static_cast<int>(mask.size()); ++z)
    {
      const std::string &row = mask[z];
      for (int x = 0; x < static_cast<int>(row.size()); ++x)
      {
        if (row[x] != 'B')
          continue;
        target.emplace_back(originX + static_cast<float>(x), y,
                            originZ + static_cast<float>(z));
      }
    }
  };

  auto dirtMask = shrinkMask(grassMask);
  auto stoneMask1 = shrinkMask(dirtMask);
  auto stoneMask2 = shrinkMask(stoneMask1);

  // camadas: pedra menor embaixo, depois terra, depois grama
  const float baseY = -4.0f; // desço tudo pra câmera não nascer colada
  applyLayer(stoneMask2, baseY - 1.0f, layout.stoneBlocks);
  applyLayer(stoneMask1, baseY, layout.stoneBlocks);
  applyLayer(dirtMask, baseY + 1.0f, layout.dirtBlocks);
  applyLayer(grassMask, baseY + 2.0f, layout.grassBlocks);

  // árvore bonitinha no canto
  const glm::vec3 treeBase(originX + 2.0f, baseY + 2.0f, originZ + 3.0f);
  for (int y = 1; y < 4; ++y)
    layout.woodBlocks.emplace_back(treeBase.x, treeBase.y + y, treeBase.z);

  // folhas padrão minecraft
  for (int y = 4; y <= 5; ++y)
  {
    for (int dx = -2; dx <= 2; ++dx)
    {
      for (int dz = -2; dz <= 2; ++dz)
      {
        const bool isEdge = std::abs(dx) == 2 || std::abs(dz) == 2;
        if (y == 5 && isEdge)
          continue;
        layout.leafBlocks.emplace_back(treeBase.x + dx, treeBase.y + y,
                                       treeBase.z + dz);
      }
    }
  }

  return layout;
}
