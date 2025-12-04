#include "../headers/glad/glad.h"
#include "../headers/glm/glm.hpp"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "../headers/Camera.hpp"
#include "../headers/Chunk.hpp"
#include "../headers/DirtBlock.hpp"
#include "../headers/GrassBlock.hpp"
#include "../headers/Light.hpp"
#include "../headers/Player.hpp"
#include "../headers/Renderer.hpp"
#include "../headers/Shader.hpp"
#include "../headers/TextureLoader.hpp"
#include "../headers/WorldGenerator.hpp"
#include "../headers/StoneBlock.hpp"
#include "../headers/WoodBlock.hpp"
#include "../headers/LeafBlock.hpp"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main() {
  // inicializar glfw
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // criar janela glfw
  GLFWwindow *window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Block Sandbox", NULL, NULL);
  if (window == NULL) {
    std::cout << "Erro ao criar janela GLFW" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, Renderer::FramebufferSizeCallback);

  Player player(SCR_WIDTH, SCR_HEIGHT);
  player.BindCallbacks(window);

  // capturar mouse com glfw
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // glad: carregar todos os ponteiros de função OpenGL
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Falha ao inicializar GLAD" << std::endl;
    return -1;
  }

  // configurar estado global do OpenGL
  glEnable(GL_DEPTH_TEST); // renderizar de trás para frente
  glEnable(GL_BLEND);      // transparência pras folhas
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  Light light;
  light.SetupSunLight();

  // compilar e linkar shaders
  Shader shader("src/resources/shaders/shader.vs",
                "src/resources/shaders/shader.fs");

  Renderer renderer;
  renderer.Initialize();

  WorldGenerator worldGenerator;
  
  // blocos exemplo
  auto layout = worldGenerator.GenerateCustomMatrix();

  TextureLoader textureLoader;
  unsigned int atlasTexture = textureLoader.LoadAtlasTexture();

  // todos os blocos usam a mesma imagem pra tentar otimizar
  GrassBlock grassBlock(atlasTexture);
  DirtBlock dirtBlock(atlasTexture);
  StoneBlock stoneBlock(atlasTexture);
  WoodBlock woodBlock(atlasTexture);
  LeafBlock leafBlock(atlasTexture);
  Chunk grassChunk(layout.grassBlocks, grassBlock);
  Chunk dirtChunk(layout.dirtBlocks, dirtBlock);
  Chunk stoneChunk(layout.stoneBlocks, stoneBlock);
  Chunk woodChunk(layout.woodBlocks, woodBlock);
  Chunk leafChunk(layout.leafBlocks, leafBlock);

  // setar texture de cada sampler
  shader.use();
  shader.setInt("block_texture", 0);

  // loop principal
  while (!glfwWindowShouldClose(window)) {
    player.UpdateFrameTime();
    player.ProcessInput(window);

    // render
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderer.Render(grassChunk.GetBlockPositions(), grassChunk.GetBlockType(),
                    shader, player.GetCamera(), SCR_WIDTH, SCR_HEIGHT);
    renderer.Render(dirtChunk.GetBlockPositions(), dirtChunk.GetBlockType(),
                    shader, player.GetCamera(), SCR_WIDTH, SCR_HEIGHT);
    renderer.Render(stoneChunk.GetBlockPositions(), stoneChunk.GetBlockType(),
                    shader, player.GetCamera(), SCR_WIDTH, SCR_HEIGHT);
    renderer.Render(woodChunk.GetBlockPositions(), woodChunk.GetBlockType(),
                    shader, player.GetCamera(), SCR_WIDTH, SCR_HEIGHT);
    renderer.Render(leafChunk.GetBlockPositions(), leafChunk.GetBlockType(),
                    shader, player.GetCamera(), SCR_WIDTH, SCR_HEIGHT);

    // glfw: swap buffers e IO
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // limpar recursos para encerrar
  renderer.Cleanup();

  // encerra glfw de vez
  glfwTerminate();
  return 0;
}
