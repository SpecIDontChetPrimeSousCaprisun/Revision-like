#include "Chloe.h"
#include "../FrenchRandom.h"
#include "../FightPhase.h"
#include "../Button.h"
#include "../TextElement.h"

Chloe::Chloe() : Professor(5.0f, "French", "Chloe", "textures/Wallpaper.jpeg", 
    "Adds 2 to the multiplier for every good answer. Removes 4 for every wrong answer.") {}

bool Chloe::registered = [] {
    Professor::getRegistry().push_back([] {
        return new Chloe();
    });
    return true;
}();

void Chloe::intervene(std::map<std::string, std::vector<std::string>> answers, std::string sentence) {
  SentenceInfo* infos = FrenchRandom::sentencesInfo[sentence];

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
        effectElement->text = "+2";
        effect->mult = 2;
      } else {
        effectElement->text = "-4";
        effectElement->textColor = glm::vec3(1.0f, 0.0f, 0.0f);
        effect->points = -4;
      }

      effectElement->registerObject();

      effect->correspondingButton = correspondingButton;
      effect->element = effectElement;
      FightPhase::effects[effect] = FrenchRandom::timeToWait;
      FrenchRandom::timeToWait += FrenchRandom::timeToAdd;
      FrenchRandom::timeToAdd -= 0.1f;
    }
  }
}
