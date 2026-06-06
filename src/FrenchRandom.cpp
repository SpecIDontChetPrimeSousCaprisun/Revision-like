#include "FrenchRandom.h"
#include "FightPhase.h"
#include <algorithm>
#include <iostream>
#include <cmath>

std::mt19937 FrenchRandom::rng(std::random_device{}());
std::vector<std::string> FrenchRandom::sentencesPool;
std::map<std::string, SentenceInfo*> FrenchRandom::sentencesInfo;

std::string FrenchRandom::getRandomSentence() {
  std::uniform_int_distribution<int> dist(0, sentencesPool.size() - 1);
  return sentencesPool[dist(rng)];
}

void FrenchRandom::init() {
  generateSentences();
  createPool();
}

void FrenchRandom::createPool() {
  sentencesPool.clear();

  for (auto& [sentence, info] : sentencesInfo) {
    sentencesPool.push_back(sentence);
  }
}

void FrenchRandom::generateSentences() {
  SentenceInfo* newInfo = new SentenceInfo();

  newInfo->types["Verbe"].push_back("mangent");
  newInfo->types["Sujet"].push_back("chevres");
  newInfo->types["COD"].push_back("l'herbe.");

  sentencesInfo["Les chevres mangent l'herbe."] = newInfo;
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

      effect->element = effectElement;
      FightPhase::effects[effect] = timeToWait;
      timeToWait += timeToAdd;
      timeToAdd -= 0.01f;
    }
  }
}
