#pragma once

#include "TextElement.h"
#include "Button.h"
#include "Container.h"

class ShopPhase {
public:
  static void init();
  static void start();
  static void update();
  static void end();
private:
  static TextElement* moneyText;
  static Container* UI;
  static std::vector<Container*> upgradeButtons;
};
