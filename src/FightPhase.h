#pragma once

#include "Container.h"

class FightPhase {
public:
  static void init();
  static void start();
private:
  static Container* UI;
};
