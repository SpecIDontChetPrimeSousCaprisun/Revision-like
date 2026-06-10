#pragma once

#include "TextElement.h"
#include "Button.h"
#include "Container.h"

struct ShopUpgrade {
public:
  Button* upgradeButton;
  TextElement* boughtElement;
};

class ShopPhase {
public:
  static void init();
  static void start();
  static void update();
  static void end();
private:
  static ShopUpgrade* addNewUpgrade(int i, float padding, size_t numberOfUpgrades, glm::vec2 position, glm::vec2 size, std::string texPath, std::string upgrade, float price);

  static TextElement* moneyText;
  static Container* UI;
  static std::vector<Container*> upgradeButtons;
};
