#include "../headers/glad/glad.h"
#include "../headers/glm/glm.hpp"
#include "../headers/glm/gtc/matrix_transform.hpp"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <vector>

#include "../headers/Camera.hpp"
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

// settings basicos de janela
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
  // inicializar glfw (janela + contexto OpenGL)
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
  if (window == NULL)
  {
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
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Falha ao inicializar GLAD" << std::endl;
    return -1;
  }

  // configurar estado global do OpenGL
  glEnable(GL_DEPTH_TEST); // renderizar de trás para frente
  glEnable(GL_BLEND);      // transparência pras folhas
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  Light light;
  light.SetupSunLight();
  light.InitShadowMap();

  // compilar e linkar shaders
  Shader shader("src/resources/shaders/shader.vs",
                "src/resources/shaders/shader.fs");
  Shader depthShader("src/resources/shaders/depth_dir.vs",
                     "src/resources/shaders/depth_dir.fs");
  Shader crosshairShader("src/resources/shaders/crosshair.vs",
                         "src/resources/shaders/crosshair.fs");

  // crosshair simples no centro da tela (2 linhas em NDC, ajustado pelo aspect)
  unsigned int crossVAO = 0, crossVBO = 0;
  {
    const float crossVerts[] = {
        -0.02f, 0.0f, 0.02f, 0.0f, // linha horizontal
        0.0f, -0.02f, 0.0f, 0.02f  // linha vertical
    };
    glGenVertexArrays(1, &crossVAO);
    glGenBuffers(1, &crossVBO);
    glBindVertexArray(crossVAO);
    glBindBuffer(GL_ARRAY_BUFFER, crossVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(crossVerts), crossVerts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
  }

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
  BlockCollection grassSet{&layout.grassBlocks, &grassBlock};
  BlockCollection dirtSet{&layout.dirtBlocks, &dirtBlock};
  BlockCollection stoneSet{&layout.stoneBlocks, &stoneBlock};
  BlockCollection woodSet{&layout.woodBlocks, &woodBlock};
  BlockCollection leafSet{&layout.leafBlocks, &leafBlock};
  std::vector<BlockCollection> allBlocks = {grassSet, dirtSet, stoneSet,
                                            woodSet, leafSet};

  // setar textura e luz (sol direcional com shadow map)
  shader.use();
  shader.setInt("block_texture", 0);
  shader.setInt("shadowMap", 1);
  light.ApplyToShader(shader.ID);
  shader.setMat4("lightSpaceMatrix", light.GetLightSpaceMatrix());

  // loop principal
  bool leftMouseWasDown = false;
  while (!glfwWindowShouldClose(window))
  {
    player.UpdateFrameTime();
    player.ProcessInput(window);
    float timeNow = static_cast<float>(glfwGetTime());

    // clique para quebrar bloco (raycast fica no player): busca bloco mais próximo no raio de 12m
    int leftState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if (leftState == GLFW_PRESS && !leftMouseWasDown)
    {
      glm::vec3 hitPos{0.0f};
      size_t hitIndex = 0;
      if (player.PickBlock(allBlocks, hitPos, hitIndex))
      {
        Player::RemoveBlockAt(*allBlocks[hitIndex].positions, hitPos);
      }
    }
    leftMouseWasDown = (leftState == GLFW_PRESS);

    // primeira passada: gerar shadow map do ponto de vista da luz (usa depthShader + sway das folhas)
    light.BeginShadowPass();
    depthShader.use();
    depthShader.setFloat("time", timeNow);
    depthShader.setMat4("lightSpaceMatrix", light.GetLightSpaceMatrix());

    // balança se for folha
    depthShader.setInt("leafBlock", 0);
    renderer.RenderDepth(*grassSet.positions, depthShader);
    renderer.RenderDepth(*dirtSet.positions, depthShader);
    renderer.RenderDepth(*stoneSet.positions, depthShader);
    renderer.RenderDepth(*woodSet.positions, depthShader);
    depthShader.setInt("leafBlock", 1);
    renderer.RenderDepth(*leafSet.positions, depthShader);
    light.EndShadowPass();

    // render normal da cena
    int fbWidth = 0, fbHeight = 0;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    glViewport(0, 0, fbWidth, fbHeight);     // usa o tamanho real do framebuffer pra não esticar
    glClearColor(0.35f, 0.55f, 0.85f, 1.0f); // céu mais suave
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();
    shader.setFloat("time", timeNow);
    shader.setMat4("lightSpaceMatrix", light.GetLightSpaceMatrix());
    light.ApplyToShader(shader.ID);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, light.GetDepthMap());

    // passa o tamanho atual pra manter proporção (só aumenta campo de visão se a janela for maior)
    // manter proporção, não distorce tudo com o tamanho
    renderer.Render(*grassSet.positions, *grassSet.block, shader,
                    player.GetCamera(), fbWidth, fbHeight);
    renderer.Render(*dirtSet.positions, *dirtSet.block, shader,
                    player.GetCamera(), fbWidth, fbHeight);
    renderer.Render(*stoneSet.positions, *stoneSet.block, shader,
                    player.GetCamera(), fbWidth, fbHeight);
    renderer.Render(*woodSet.positions, *woodSet.block, shader,
                    player.GetCamera(), fbWidth, fbHeight);
    renderer.Render(*leafSet.positions, *leafSet.block, shader,
                    player.GetCamera(), fbWidth, fbHeight);

    // crosshair 2D no centro
    glDisable(GL_DEPTH_TEST);
    crosshairShader.use();
    crosshairShader.setVec3("color", glm::vec3(1.0f, 1.0f, 1.0f));
    glBindVertexArray(crossVAO);

    // ajusta com aspect ratio pra manter o + simétrico em qualquer resolução
    float aspect = static_cast<float>(fbWidth) / static_cast<float>(fbHeight);
    if (aspect >= 1.0f)
    {
      glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f / aspect, 1.0f, 1.0f));
      crosshairShader.setMat4("projection", scale);
    }
    else
    {
      glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, aspect, 1.0f));
      crosshairShader.setMat4("projection", scale);
    }
    glDrawArrays(GL_LINES, 0, 4);
    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);

    // glfw: swap buffers e IO
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // limpar recursos para encerrar
  renderer.Cleanup();
  glDeleteVertexArrays(1, &crossVAO);
  glDeleteBuffers(1, &crossVBO);

  // encerra glfw de vez
  glfwTerminate();
  return 0;
}
