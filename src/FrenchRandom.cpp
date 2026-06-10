#include "FrenchRandom.h"
#include "FightPhase.h"
#include "Gameloop.h"
#include <algorithm>
#include <iostream>
#include <cmath>
#include <nlohmann/json.hpp>
#include <fstream>

std::mt19937 FrenchRandom::rng(std::random_device{}());
std::vector<UpgradeInfo*> FrenchRandom::upgradePool;
std::vector<std::string> FrenchRandom::sentencesPool;
std::map<std::string, SentenceInfo*> FrenchRandom::sentencesInfo;

std::string FrenchRandom::getRandomSentence() {
  createPool();

  std::uniform_int_distribution<int> dist(0, sentencesPool.size() - 1);
  return sentencesPool[dist(rng)];
}

std::vector<UpgradeInfo*> FrenchRandom::getRandomUpgrades() {
  std::vector<UpgradeInfo*> toRemove;

  for (UpgradeInfo* upgrade : upgradePool) {
    if (std::find(Gameloop::upgrades.begin(), Gameloop::upgrades.end(), upgrade->upgrade) != Gameloop::upgrades.end()) {
      toRemove.push_back(upgrade);
    }
  }

  for (UpgradeInfo* upgrade : toRemove) {
    upgradePool.erase(
      std::remove(upgradePool.begin(), upgradePool.end(), upgrade),
      upgradePool.end()
    );
  }

  std::vector<UpgradeInfo*> currentPool = upgradePool;
  std::vector<UpgradeInfo*> result;

  for (int i = 0; i < 5; i++) {
    if (currentPool.empty()) break;

    std::uniform_int_distribution<int> dist(0, currentPool.size() - 1);
    int index = dist(rng);

    result.push_back(currentPool[index]);
    currentPool.erase(currentPool.begin() + index);
  }

  return result;
}

void FrenchRandom::init() {
  generateSentences(); 
  createUpgradePool();
}

void FrenchRandom::createPool() {
  sentencesPool.clear();

  for (auto& [sentence, info] : sentencesInfo) {
    bool requirementsMet = true;

    for (std::string requirement : info->requirements) {
      if (std::find(Gameloop::upgrades.begin(), Gameloop::upgrades.end(), requirement) == Gameloop::upgrades.end()) {
        requirementsMet = false;
        break;
      }
    }

    if (!requirementsMet) continue;

    sentencesPool.push_back(sentence);
  }
}

void FrenchRandom::createUpgradePool() {
  std::ifstream upgradesFile("infos/FrenchUpgrades.json");

  nlohmann::json upgrades;
  upgradesFile >> upgrades;

  for (auto& [key, value] : upgrades.items()) {
    UpgradeInfo* infos = new UpgradeInfo();

    infos->upgrade = key;
    infos->price = value["price"];
    infos->texPath = value["texPath"];

    upgradePool.push_back(infos);
  }
}

void FrenchRandom::generateSentences() {
  std::ifstream sentencesFile("infos/FrenchSentences.json");

  nlohmann::json sentences;
  sentencesFile >> sentences;

  for (auto& [sentence, info] : sentences.items()) {
    SentenceInfo* newInfo = new SentenceInfo();

    for (auto& [type, values] : info["types"].items()) {
      for (auto& value : values) {
        newInfo->types[type].push_back(value);
      }
    }

    for (auto& requirement : info["requirements"]) {
      newInfo->requirements.push_back(requirement);
    }

    sentencesInfo[sentence] = newInfo;
  }
}

void FrenchRandom::evaluatePoints(std::map<std::string, std::vector<std::string>> answers, std::string sentence) {
  SentenceInfo* infos = sentencesInfo[sentence];
  float timeToWait = 0.0f;
  float timeToAdd = 1.0f;

  for (auto& [word, options] : answers) {
    for (std::string answer : options) {
      std::string str = word;
      str.erase(
          std::remove(str.begin(), str.end(), ' '),
          str.end()
      );

      Button* correspondingButton;

      for (Button* button : FightPhase::expressionButtons) {
        if (button->text == word) {
          correspondingButton = button;
          break;
        }
      }

      TextElement* effectElement = new TextElement(correspondingButton->position, glm::vec2(correspondingButton->size.x, 0.025f), 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), 15, "", "fonts/Kenney Future Narrow.ttf", glm::vec3(0.0f, 1.0f, 0.0f));
      Effect* effect = new Effect();

      effectElement->anchorPoint = glm::vec2(0.0f, 1.0f);
      effectElement->visible = false;

      if (std::find(infos->types[answer].begin(), infos->types[answer].end(), str) != infos->types[answer].end()) {
        effectElement->text = "+5";
        effect->points = 5;
      } else {
        effectElement->text = "-5";
        effectElement->textColor = glm::vec3(1.0f, 0.0f, 0.0f);
        effect->points = -5;
      }

      effectElement->registerObject();

      effect->correspondingButton = correspondingButton;
      effect->element = effectElement;
      FightPhase::effects[effect] = timeToWait;
      timeToWait += timeToAdd;
      timeToAdd -= 0.1f;
    }
  }
}
