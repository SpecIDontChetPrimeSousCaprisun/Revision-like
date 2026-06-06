#include "ShopPhase.h"
#include "Gameloop.h"
#include <vector>
#include <fstream>
#include <iomanip>

TextElement* ShopPhase::moneyText;
Container* ShopPhase::UI;

void ShopPhase::init() {
  std::vector<Object*> UIElements;

  moneyText = new TextElement(glm::vec2(0.01f, 1.0f), glm::vec2(1.0f, 0.1f), 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), 2, "0$", "fonts/Kenney Future Narrow.ttf", glm::vec3(0.0f, 1.0f, 0.0f));

  moneyText->anchorPoint = glm::vec2(0.0f, 1.0f);
  moneyText->textCentered = false;

  UIElements.push_back(moneyText);

  UI = new Container(UIElements);
  UI->changeVisibility(false);
  UI->registerObjects();
}

void ShopPhase::start() {
  UI->changeVisibility(true);
}

void ShopPhase::update() {
  std::ostringstream moneyss;
  moneyss << std::fixed << std::setprecision(1) << Gameloop::money;
  moneyText->text = moneyss.str() + "$";
}
