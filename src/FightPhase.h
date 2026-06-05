#pragma once

#include "Container.h"
#include "TextElement.h"
#include "Textbox.h"
#include "Button.h"
#include "ScrollingElement.h"
#include <vector>

class FightPhase {
public:
  static void init();
  static void start();
  static void update();
private:
  static std::vector<Button*> optionButtons;
  static Button* selectedExpression;
  static Container* UI;
  static TextElement* scoreLabel;
  static TextElement* moneyGainLabel;
  static Textbox* searchBox;
  static ScrollingElement* optionsScroll;
};
