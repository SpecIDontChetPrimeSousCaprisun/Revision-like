#pragma once

#include <map>
#include <string>

class Gameloop {
public:
  static void init();
  static void stepFoward();

  static bool looping, completedStage;
  static int currentStage;
private:
  static std::map<int, std::string> stages;
};
