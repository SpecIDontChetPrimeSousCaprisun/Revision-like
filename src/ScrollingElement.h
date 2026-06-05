#pragma once

#include "UIElement.h"
#include <vector>

class ScrollingElement : public UIElement {
public:
  ScrollingElement(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, int zIndex);
  ScrollingElement(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, int zIndex);

  std::vector<UIElement*> elements;
  float scrollAmount;
protected:
  void beforeUpdate() override;
};
