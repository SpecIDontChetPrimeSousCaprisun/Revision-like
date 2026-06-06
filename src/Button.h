#pragma once

#include "TextElement.h"
#include <functional>

class Button : public TextElement {
public:
  Button(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, int zIndex,
         std::string text, std::string fontPath, glm::vec3 textColor);
  Button(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, int zIndex,
         std::string text, std::string fontPath, glm::vec3 textColor);

  void setCallback(std::function<void()> callback);

  bool interactible;
protected:
  drawInfo* beforeDrawing(drawInfo* info) override;
  void beforeUpdate() override;
private:
  std::function<void()> callback;
  bool pressed, ignoreCurrentInput;
};
