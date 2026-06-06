#pragma once

#include "TextElement.h"
#include "Container.h"

class ShopPhase {
public:
  static void init();
  static void start();
  static void update();
private:
  static TextElement* moneyText;
  static Container* UI;
};
