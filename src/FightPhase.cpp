#include "FightPhase.h"
#include "UIElement.h"
#include "TextElement.h"
#include "Button.h"

Container* FightPhase::UI;

void FightPhase::init() {
  std::vector<Object*> UIElements;

  UIElement* sidePannel = new UIElement(glm::vec2(0.0f, 0.0f), glm::vec2(0.25f, 1.0f), 0.0f, glm::vec3(0.3f, 0.0f, 0.3f), 2);

  UIElements.push_back(sidePannel);

  UI = new Container(UIElements);
  UI->registerObjects();
  UI->changeVisibility(false);
}

void FightPhase::start() {
  UI->changeVisibility(true);
}
