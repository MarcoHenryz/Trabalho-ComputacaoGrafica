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

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main() {
  // initialize and configure glfw
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // glfw window creation
  GLFWwindow *window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Block Sandbox", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, Renderer::FramebufferSizeCallback);

  Player player(SCR_WIDTH, SCR_HEIGHT);
  player.BindCallbacks(window);

  // capture mouse with glfw
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // glad: load all OpenGL function pointers
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // configure global OpenGL state
  glEnable(GL_DEPTH_TEST); // render from back to front

  Light light;
  light.SetupSunLight();

  // build and compile our shaders program
  Shader shader("src/resources/shaders/shader.vs",
                "src/resources/shaders/shader.fs");

  Renderer renderer;
  renderer.Initialize();

  WorldGenerator worldGenerator;
  auto cubePositions = worldGenerator.GenerateFlatChunk();

  TextureLoader textureLoader;
  unsigned int dirt_texture = textureLoader.LoadDirtTexture();

  DirtBlock dirtBlock(dirt_texture);
  GrassBlock grassBlock;
  (void)grassBlock;
  Chunk chunk(cubePositions, dirtBlock);

  // tell opengl for each sampler to which texture unit it belongs to
  shader.use();
  glUniform1i(glGetUniformLocation(shader.ID, "dirt_texture"), 0);
  shader.setInt("dirt_texture", 0);

  // render loop
  while (!glfwWindowShouldClose(window)) {
    player.UpdateFrameTime();
    player.ProcessInput(window);

    // render
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderer.Render(chunk.GetBlockPositions(), shader, player.GetCamera(),
                    chunk.GetBlockType().GetTextureId(), SCR_WIDTH, SCR_HEIGHT);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse move)
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // de-allocate all resources once they've outlived their purpose:
  renderer.Cleanup();

  // glfw: terminate, clearing all previously allocated GLFW resources.
  glfwTerminate();
  return 0;
}
