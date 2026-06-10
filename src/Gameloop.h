#pragma once

#include <map>
#include <string>
#include <vector>
#include "professors/Professor.h"

class Gameloop {
public:
  static void init();
  static void stepFoward();
  static void update();

  static bool looping, completedStage;
  static int currentStage;
  static float money;
  static std::string currentSubject;
  static std::map<int, std::string> stages;
  static std::vector<std::string> upgrades;
  static std::vector<Professor*> professors;
};
