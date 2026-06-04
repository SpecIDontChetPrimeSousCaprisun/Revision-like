#include "FightPhase.h"
#include "UIElement.h"
#include "Button.h"
#include "FrenchRandom.h"
#include "Gameloop.h"

Container* FightPhase::UI;
TextElement* FightPhase::scoreLabel;
TextElement* FightPhase::moneyGainLabel;

void FightPhase::init() {
  FrenchRandom::init();

  std::vector<Object*> UIElements;

  UIElement* sidePannel = new UIElement(glm::vec2(0.0f, 0.0f), glm::vec2(0.25f, 1.0f), 0.0f, glm::vec3(0.3f, 0.0f, 0.3f), 2);
  scoreLabel = new TextElement(glm::vec2(0.0f, 0.1f), glm::vec2(0.25f, 0.1f), 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), 3, "0/20", "fonts/Kenney Future Narrow.ttf", glm::vec3(1.0f, 1.0f, 1.0f));
  moneyGainLabel = new TextElement(glm::vec2(0.0f, 0.2f), glm::vec2(0.25f, 0.1f), 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), 3, "0$", "fonts/Kenney Future Narrow.ttf", glm::vec3(0.0f, 1.0f, 0.0f));


  UIElements.push_back(sidePannel);
  UIElements.push_back(scoreLabel);
  UIElements.push_back(moneyGainLabel);

  UI = new Container(UIElements);
  UI->registerObjects();
  UI->changeVisibility(false);
}

void FightPhase::start() {
  UI->changeVisibility(true);

  if (Gameloop::currentSubject == "French") {
    std::cout << FrenchRandom::getRandomSentence() << "\n";
  }
}
