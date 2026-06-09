#include "ScrollingElement.h"

ScrollingElement::ScrollingElement(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, int zIndex)
  : UIElement(position, size, transparency, texPath, zIndex), scrollAmount(0.0f), padding(0.0f) {}

ScrollingElement::ScrollingElement(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, int zIndex)
  : UIElement(position, size, transparency, color, zIndex), scrollAmount(0.0f), padding(0.0f) {}

void ScrollingElement::beforeUpdate() {
  float y = 0;

  for (UIElement* element : elements) {
    element->size.x = size.x;
    element->position = glm::vec2(position.x + padding, position.y + y + padding);
    element->anchorPoint = anchorPoint;

    if (!visible) element->visible = false;
    if (element->visible) y += element->size.y; 
  }
}
