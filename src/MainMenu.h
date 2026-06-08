#pragma once

#include "TextElement.h"
#include "Container.h"

class MainMenu {
public:
  static void init();
  static void update();
private:
  static float titleSpeed;
  static TextElement* title;
  static Container* mainButtonsContainer;
  static Container* subjectButtonsContainer;
};
