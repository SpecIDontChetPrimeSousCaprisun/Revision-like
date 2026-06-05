#include "FightPhase.h"
#include "UIElement.h"
#include "FrenchRandom.h"
#include "Gameloop.h"
#include "Window.h"
#include <sstream>
#include <nlohmann/json.hpp>
#include <fstream>

Button* FightPhase::selectedExpression;
Container* FightPhase::UI;
TextElement* FightPhase::scoreLabel;
TextElement* FightPhase::moneyGainLabel;
Textbox* FightPhase::searchBox;
ScrollingElement* FightPhase::optionsScroll;
std::vector<Button*> FightPhase::optionButtons;

void FightPhase::init() {
  FrenchRandom::init();

  std::vector<Object*> UIElements;

  UIElement* sidePannel = new UIElement(glm::vec2(0.0f, 0.0f), glm::vec2(0.25f, 1.0f), 0.0f, glm::vec3(0.3f, 0.0f, 0.3f), 2);
  scoreLabel = new TextElement(glm::vec2(0.0f, 0.1f), glm::vec2(0.25f, 0.1f), 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), 3, "0/20", "fonts/Kenney Future Narrow.ttf", glm::vec3(1.0f, 1.0f, 1.0f));
  moneyGainLabel = new TextElement(glm::vec2(0.0f, 0.2f), glm::vec2(0.25f, 0.1f), 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), 3, "0$", "fonts/Kenney Future Narrow.ttf", glm::vec3(0.0f, 1.0f, 0.0f));
  searchBox = new Textbox(glm::vec2(0.5f, 0.25f), glm::vec2(0.75f, 0.05f), 0.0f, glm::vec3(0.4f, 0.0f, 0.4f), 10, "fonts/Kenney Future Narrow.ttf", glm::vec3(1.0f, 1.0f, 1.0f));
  optionsScroll = new ScrollingElement(glm::vec2(0.5f, 0.3f), glm::vec2(0.75f, 0.2f), 0.0f, glm::vec3(0.4f, 0.0f, 0.4f), 10);

  searchBox->anchorPoint = glm::vec2(0.5f, 0.0f);
  searchBox->visible = false;
  searchBox->textCentered = false;
  searchBox->placeholder = "Search...";
  searchBox->registerObject();

  optionsScroll->visible = false;
  optionsScroll->anchorPoint = glm::vec2(0.5f, 0.0f);
  optionsScroll->registerObject();

  UIElements.push_back(sidePannel);
  UIElements.push_back(scoreLabel);
  UIElements.push_back(moneyGainLabel);

  UI = new Container(UIElements);
  UI->registerObjects();
  UI->changeVisibility(false);
}

void FightPhase::start() {
  UI->changeVisibility(true);
  optionButtons.clear();

  std::ifstream optionsFile("infos/" + Gameloop::currentSubject + "Options.json");
  
  nlohmann::json options;
  optionsFile >> options;

  for (auto& [key, value] : options.items()) {
    Button* optionButton = new Button(glm::vec2(0.265f, 0.375f), glm::vec2(0.25f, 0.025f), 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), 11, key, "fonts/Kenney Future Narrow.ttf", glm::vec3(value["color"][0], value["color"][1], value["color"][2]));

    optionButton->setCallback([value]() {
      Button* expressionButton = new Button(selectedExpression->position + glm::vec2(0.0f, selectedExpression->size.y), glm::vec2(selectedExpression->textWidth / Window::fbWidth, 0.025f), 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), 3, value["abreviation"], "fonts/Kenney Future Narrow.ttf", glm::vec3(value["color"][0], value["color"][1], value["color"][2]));

      expressionButton->registerObject();

      searchBox->visible = false;
      searchBox->setFocused(false);
    });

    optionButton->textCentered = false;
    optionButton->registerObject();

    optionButtons.push_back(optionButton);
    optionsScroll->elements.push_back(optionButton);
  }

  if (Gameloop::currentSubject == "French") {
    float xOffset = 0.0f;

    std::istringstream stream(FrenchRandom::getRandomSentence());
    std::string word;

    while (stream >> word)  {
      Button* expressionButton = new Button(glm::vec2(0.265f + (xOffset / Window::fbWidth), 0.375f), glm::vec2(0.25f, 0.035f), 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), 3, word + " ", "fonts/Kenney Future Narrow.ttf", glm::vec3(1.0f, 1.0f, 1.0f));
 
      expressionButton->setCallback([word, expressionButton]() {
        searchBox->visible = true;
        searchBox->setFocused(true); 
        selectedExpression = expressionButton;
      });

      expressionButton->textCentered = false;
      expressionButton->recalculateTextWidth();
      xOffset += expressionButton->textWidth;
      expressionButton->size.x = expressionButton->textWidth / Window::fbWidth;
      expressionButton->registerObject();
    }
  }
}

std::string toLower(std::string str) {
    std::transform(
        str.begin(),
        str.end(),
        str.begin(),
        [](unsigned char c) {
            return std::tolower(c);
        }
    );

    return str;
}

void FightPhase::update() {
  optionsScroll->visible = searchBox->isFocused();

  if (!searchBox->text.empty()) {
    for (Button* button : optionButtons) {
      button->visible = toLower(searchBox->text) == toLower(button->text.substr(0, searchBox->text.size()));
    }
  } else {
    for (Button* button : optionButtons) {
      button->visible = true;
    }
  }
}
