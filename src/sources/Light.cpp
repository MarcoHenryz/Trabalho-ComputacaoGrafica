#include "../headers/Light.hpp"
#include "../headers/glm/gtc/matrix_transform.hpp"
#include "glad/glad.h"

Light::Light() = default;

void Light::SetupSunLight()
{
  // sol inclinado pra ficar dramático
  direction = glm::normalize(glm::vec3(0.3f, -1.0f, 0.3f));
  color = glm::vec3(1.0f, 0.95f, 0.85f);
  ambientStrength = 0.3f;

  // projeção ortográfica cobre a ilha e a árvore (luz direcional usa ortho)
  lightProjection =
      glm::ortho(-25.0f, 25.0f, -15.0f, 25.0f, -10.0f, 40.0f);

  // sol longe olhando pro centro
  glm::vec3 lightPos = -direction * 20.0f;
  lightView = glm::lookAt(lightPos, glm::vec3(0.0f, -4.0f, 0.0f),
                          glm::vec3(0.0f, 1.0f, 0.0f));
  lightSpaceMatrix = lightProjection * lightView;
}

void Light::InitShadowMap()
{
  // FBO só de profundidade para o shadow map (sombra dura, sem cor)
  glGenFramebuffers(1, &depthMapFBO);
  glGenTextures(1, &depthMap);
  glBindTexture(GL_TEXTURE_2D, depthMap);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH,
               SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

  // borda 1.0 evita artefato nas bordas do shadow map
  float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

  glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                         depthMap, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Light::BeginShadowPass()
{
  // direciona viewport e framebuffer para escrever profundidade do sol
  glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
  glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
  glClear(GL_DEPTH_BUFFER_BIT);
}

void Light::EndShadowPass() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void Light::ApplyToShader(unsigned int shaderID) const
{
  // seta uniforms básicos no pass normal (direção, cor, força, matriz e sampler)
  glUniform3fv(glGetUniformLocation(shaderID, "lightDir"), 1, &direction[0]);
  glUniform3fv(glGetUniformLocation(shaderID, "lightColor"), 1, &color[0]);
  glUniform1f(glGetUniformLocation(shaderID, "ambientStrength"),
              ambientStrength);
  glUniformMatrix4fv(glGetUniformLocation(shaderID, "lightSpaceMatrix"), 1,
                     GL_FALSE, &lightSpaceMatrix[0][0]);
  glUniform1i(glGetUniformLocation(shaderID, "shadowMap"), 1);
}
