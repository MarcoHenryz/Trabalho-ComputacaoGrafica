#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "glm/glm.hpp"
#include "glad/glad.h"

// luz direcional simples + shadow map 2D (sombra dura)
class Light {
public:
  Light();
  void SetupSunLight();          // direção e matriz (~45 graus)
  void InitShadowMap();          // cria FBO + textura de profundidade
  void BeginShadowPass();        // bind do FBO e viewport do shadow
  void EndShadowPass();          // volta pro framebuffer padrão
  void ApplyToShader(unsigned int shaderID) const; // seta uniforms no pass normal

  const glm::mat4 &GetLightSpaceMatrix() const { return lightSpaceMatrix; }
  unsigned int GetDepthMap() const { return depthMap; }

private:
  glm::vec3 direction{0.0f};
  glm::vec3 color{1.0f};
  float ambientStrength = 0.3f;

  glm::mat4 lightProjection{1.0f};
  glm::mat4 lightView{1.0f};
  glm::mat4 lightSpaceMatrix{1.0f};

  unsigned int depthMapFBO = 0;
  unsigned int depthMap = 0;
  const int SHADOW_WIDTH = 2048;
  const int SHADOW_HEIGHT = 2048;
};

#endif
