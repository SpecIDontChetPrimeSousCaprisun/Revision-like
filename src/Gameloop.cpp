#include "Gameloop.h"
#include "FightPhase.h"
#include "ShopPhase.h"

bool Gameloop::looping = false;
bool Gameloop::completedStage = true;
int Gameloop::currentStage = 0;
float Gameloop::money = 0.0f;
std::string Gameloop::currentSubject = "";
std::map<int, std::string> Gameloop::stages;
std::vector<std::string> Gameloop::upgrades;
std::vector<Professor*> Gameloop::professors;

void Gameloop::init() {
  stages[1] = "Fight";
  stages[2] = "Shop";

  FightPhase::init();
  ShopPhase::init();
}

void Gameloop::stepFoward() {
  if (!looping) return;
  if (!completedStage) return;

  completedStage = false;
  currentStage++;
  
  if (currentStage > stages.size() - 1) currentStage = 1;

  if (stages[currentStage] == "Fight") {
    FightPhase::start();
  } else if (stages[currentStage] == "Shop") {
    ShopPhase::start();
  }
}

void Gameloop::update() {
  if (stages[currentStage] == "Fight") {
    FightPhase::update();
  } else if (stages[currentStage] == "Shop") {
    ShopPhase::update();
  }
}
