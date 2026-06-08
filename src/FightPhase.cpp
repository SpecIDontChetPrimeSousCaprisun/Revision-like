#include "FightPhase.h"
#include "UIElement.h"
#include "FrenchRandom.h"
#include "Gameloop.h"
#include "Window.h"
#include <sstream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <cmath>

float FightPhase::points = 0.0f;
float FightPhase::maxPoints = 20.0f;
std::unordered_map<Effect*, float> FightPhase::effects;
std::vector<Button*> FightPhase::expressionButtons;
std::string FightPhase::expression;
std::map<std::string, std::vector<Button*>> FightPhase::answers;
std::map<std::string, std::vector<std::string>> FightPhase::answerNames;
Button* FightPhase::selectedExpression;
Button* FightPhase::endTurnButton;
Container* FightPhase::UI;
Container* FightPhase::endUI;
TextElement* FightPhase::scoreLabel;
TextElement* FightPhase::moneyGainLabel;
ScrollingElement* FightPhase::endScroll;
Button* FightPhase::endButton;
Textbox* FightPhase::searchBox;
ScrollingElement* FightPhase::optionsScroll;
std::vector<Button*> FightPhase::optionButtons;

void FightPhase::init() {
  FrenchRandom::init();

  std::vector<Object*> UIElements;

  UIElement* sidePannel = new UIElement(glm::vec2(0.0f, 0.0f), glm::vec2(0.25f, 1.0f), 0.0f, glm::vec3(0.3f, 0.0f, 0.3f), 2);
  endTurnButton = new Button(glm::vec2(0.125f, 0.88f), glm::vec2(0.225f, 0.075f), 0.0f, glm::vec3(0.512f, 0.0f, 0.750f), 3, "End turn", "fonts/Kenney Future Narrow.ttf", glm::vec3(1.0f, 1.0f, 1.0f));
  scoreLabel = new TextElement(glm::vec2(0.0f, 0.01f), glm::vec2(0.25f, 0.1f), 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), 3, "0/20", "fonts/Kenney Future Narrow.ttf", glm::vec3(1.0f, 1.0f, 1.0f));
  moneyGainLabel = new TextElement(glm::vec2(0.0f, 0.06f), glm::vec2(0.25f, 0.1f), 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), 3, "0$", "fonts/Kenney Future Narrow.ttf", glm::vec3(0.0f, 1.0f, 0.0f));
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

  endTurnButton->setCallback([endTurnButton]() {
    endTurnButton->visible = false;

    for (Button* button : expressionButtons) {
      button->interactible = false;
    }

    for (Button* button : optionButtons) {
      button->interactible = false;
    }

    for (auto& [key, buttons] : answers) {
      for (Button* button : buttons) {
        button->interactible = false;
      }
    }

    if (Gameloop::currentSubject == "French") {
      FrenchRandom::evaluatePoints(answerNames, expression);
    }
  });
  endTurnButton->anchorPoint = glm::vec2(0.5f, 0.0f);

  UIElements.push_back(sidePannel);
  UIElements.push_back(scoreLabel);
  UIElements.push_back(moneyGainLabel);
  UIElements.push_back(endTurnButton);

  UI = new Container(UIElements);
  UI->registerObjects();
  UI->changeVisibility(false);

  // End UI
  std::vector<Object*> endUIElements;

  UIElement* endBackground = new UIElement(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), 0.25f, glm::vec3(0.0f, 0.0f, 0.0f), 20);
  UIElement* endContainer = new UIElement(glm::vec2(0.5f, 0.5f), glm::vec2(0.75f, 0.9f), 0.0f, glm::vec3(0.512f, 0.0f, 0.750f), 21);
  TextElement* title = new TextElement(glm::vec2(0.5f, 0.05f), glm::vec2(0.75f, 0.1f), 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), 22, "Fight breakdown", "fonts/Kenney Future Narrow.ttf", glm::vec3(1.0f, 1.0f, 1.0f));
  endScroll = new ScrollingElement(glm::vec2(0.5f, 0.15f), glm::vec2(0.7f, 0.65f), 0.0f, glm::vec3(0.4f, 0.0f, 0.4f), 22);
  endButton = new Button(glm::vec2(0.5f, 0.85f), glm::vec2(0.7f, 0.075f), 0.0f, glm::vec3(0.4f, 0.0f, 0.4f), 22, "Continue", "fonts/Kenney Future Narrow.ttf", glm::vec3(1.0f, 1.0f, 1.0f));

  endContainer->anchorPoint = glm::vec2(0.5f, 0.5f);
  
  title->anchorPoint = glm::vec2(0.5f, 0.0f);

  endScroll->anchorPoint = glm::vec2(0.5f, 0.0f);

  endButton->anchorPoint = glm::vec2(0.5f, 0.0f);
  endButton->setCallback([]() {
    end();
  });

  endUIElements.push_back(endBackground);
  endUIElements.push_back(endContainer);
  endUIElements.push_back(title);
  endUIElements.push_back(endScroll);
  endUIElements.push_back(endButton);

  endUI = new Container(endUIElements);
  endUI->registerObjects();
  endUI->changeVisibility(false);
}

void FightPhase::start() {
  UI->changeVisibility(true);
  endUI->changeVisibility(false);
  optionButtons.clear();
  expressionButtons.clear();
  answers.clear();
  answerNames.clear();
  effects.clear();

  std::ifstream optionsFile("infos/" + Gameloop::currentSubject + "Options.json");
  
  nlohmann::json options;
  optionsFile >> options;

  for (auto& [key, value] : options.items()) {
    bool requirementsNotMet = false;

    for (const auto& requirement : value["requirements"]) {
      if (std::find(Gameloop::upgrades.begin(), Gameloop::upgrades.end(), requirement) == Gameloop::upgrades.end()) {
        requirementsNotMet = true;
        break;
      }
    }

    if (requirementsNotMet) continue;

    Button* optionButton = new Button(glm::vec2(0.265f, 0.375f), glm::vec2(0.25f, 0.025f), 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), 11, key, "fonts/Kenney Future Narrow.ttf", glm::vec3(value["color"][0], value["color"][1], value["color"][2]));

    optionButton->setCallback([key, value]() {
      std::string selectedExpressionText = selectedExpression->text;
      Button* expressionButton = new Button(selectedExpression->position + glm::vec2(0.0f, selectedExpression->size.y), glm::vec2(selectedExpression->textWidth / Window::fbWidth, 0.025f), 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), 3, value["abreviation"], "fonts/Kenney Future Narrow.ttf", glm::vec3(value["color"][0], value["color"][1], value["color"][2]));

      expressionButton->setCallback([expressionButton, selectedExpressionText, key]() {
        expressionButton->pendDelete();
        answers[selectedExpressionText].erase(
          std::remove(answers[selectedExpressionText].begin(), answers[selectedExpressionText].end(), expressionButton),
          answers[selectedExpressionText].end()
        );
        
        answerNames[selectedExpressionText].erase(
          std::remove(answerNames[selectedExpressionText].begin(), answerNames[selectedExpressionText].end(), key),
          answerNames[selectedExpressionText].end()
        );

        for (Button* button : answers[selectedExpressionText]) {
          if (button->position.y > expressionButton->position.y) {
            button->position.y -= 0.025f;
          }
        }
      });

      expressionButton->position.y += 0.025f * answers[selectedExpressionText].size();
      expressionButton->registerObject();
      answers[selectedExpressionText].push_back(expressionButton);
      answerNames[selectedExpressionText].push_back(key);

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

    expression = FrenchRandom::getRandomSentence();
    std::istringstream stream(expression);
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
      expressionButtons.push_back(expressionButton);
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

  std::ostringstream pointss;
  std::ostringstream maxPointss;
  std::ostringstream moneyss;
  pointss << std::fixed << std::setprecision(1) << points;
  maxPointss << std::fixed << std::setprecision(1) << maxPoints;
  moneyss << std::fixed << std::setprecision(1) << std::max(0.0f, points - (maxPoints / 2));

  scoreLabel->text = pointss.str() + "/" + maxPointss.str();
  moneyGainLabel->text = moneyss.str() + "$";

  if (!searchBox->text.empty()) {
    for (Button* button : optionButtons) {
      button->visible = toLower(searchBox->text) == toLower(button->text.substr(0, searchBox->text.size()));
    }
  } else {
    for (Button* button : optionButtons) {
      button->visible = true;
    }
  }

  std::vector<Effect*> toRemove;

  for (auto& [effect, time] : effects) {
    time -= Window::deltaTime;
    if (time <= 0.0f) {
      effect->element->visible = true;
      
      if (effect->fadeOut) {
        effect->element->textTransparency = time / -0.5f;

        if (time <= -0.5f) {
          effect->element->pendDelete();
          toRemove.push_back(effect);
        }
      }

      if (!effect->appliedPoints) {
        effect->appliedPoints = true;
        points = std::min(points + effect->points, maxPoints);
      }
    }
  }

  for (Effect* effect : toRemove) {
    effects.erase(effect);
  }

  if (effects.size() == 0 && !endTurnButton->visible && Gameloop::stages[Gameloop::currentStage] == "Fight" && !endScroll->visible) {
    endUI->changeVisibility(true);

    TextElement* endScore = new TextElement(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.035f), 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), 23, pointss.str() + "/" + maxPointss.str(), "fonts/Kenney Future Narrow.ttf", glm::vec3(1.0f, 1.0f, 1.0f));
    TextElement* endMoney = new TextElement(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.035f), 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), 23, moneyss.str() + "$", "fonts/Kenney Future Narrow.ttf", glm::vec3(0.0f, 1.0f, 0.0f));
  
    endScore->visible = false;
    endScore->textCentered = false;
    endScore->registerObject();

    endMoney->visible = false;
    endMoney->textCentered = false;
    endMoney->registerObject();

    endScroll->elements.push_back(endScore);
    endScroll->elements.push_back(endMoney);

    Effect* endScoreEffect = new Effect();
    Effect* endMoneyEffect = new Effect();

    endScoreEffect->points = 0;
    endScoreEffect->fadeOut = false;
    endScoreEffect->element = endScore;

    endMoneyEffect->points = 0;
    endMoneyEffect->fadeOut = false;
    endMoneyEffect->element = endMoney;

    effects[endScoreEffect] = 1.0f;
    effects[endMoneyEffect] = 2.0f;
  }
}

void FightPhase::end() {
  Gameloop::completedStage = true;
  Gameloop::money = std::max(Gameloop::money + (points - (maxPoints / 2)), Gameloop::money);
  points = 0.0f;
  UI->changeVisibility(false);
  endUI->changeVisibility(false);
  searchBox->visible = false;

  for (Button* button : expressionButtons) {
    button->pendDelete();
  }

  for (Button* button : optionButtons) {
    button->pendDelete();
  }

  for (auto& [key, buttons] : answers) {
    for (Button* button : buttons) {
      button->pendDelete();
    }
  }

  for (UIElement* element : endScroll->elements) {
    element->pendDelete();
  }

  optionsScroll->elements.clear();
  endScroll->elements.clear();
}
