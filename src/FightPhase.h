#pragma once

#include "Container.h"
#include "TextElement.h"
#include "Textbox.h"

class FightPhase {
public:
  static void init();
  static void start();
private:
  static Container* UI;
  static TextElement* scoreLabel;
  static TextElement* moneyGainLabel;
  static Textbox* searchBox;
};
