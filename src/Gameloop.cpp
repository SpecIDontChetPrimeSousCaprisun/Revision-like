#include "Gameloop.h"
#include "FightPhase.h"

bool Gameloop::looping = false;
bool Gameloop::completedStage = true;
int Gameloop::currentStage = 0;
std::map<int, std::string> Gameloop::stages;

void Gameloop::init() {
  stages[1] = "Fight";

  FightPhase::init();
}

void Gameloop::stepFoward() {
  if (!looping) return;
  if (!completedStage) return;

  currentStage++;
  
  if (currentStage > stages.size()) currentStage = 1;

  if (stages[currentStage] == "Fight") {
    FightPhase::start();
  }
}
