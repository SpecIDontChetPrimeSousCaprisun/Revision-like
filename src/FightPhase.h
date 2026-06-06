#pragma once

#include "Container.h"
#include "TextElement.h"
#include "Textbox.h"
#include "Button.h"
#include "ScrollingElement.h"
#include <vector>
#include <map>
#include <unordered_map>

class Effect {
public:
  TextElement* element;
  float points;
  bool appliedPoints = false;
  bool fadeOut = true;
};

class FightPhase {
public:
  static void init();
  static void start();
  static void end();
  static void update();

  static float points, maxPoints;
  static std::unordered_map<Effect*, float> effects;
  static std::vector<Button*> expressionButtons;
private: 
  static std::map<std::string, std::vector<Button*>> answers;
  static std::map<std::string, std::vector<std::string>> answerNames;
  static std::vector<Button*> optionButtons;
  static std::string expression;
  static Button* selectedExpression;
  static Button* endTurnButton;
  static Container* UI;
  static Container* endUI;
  static TextElement* scoreLabel;
  static TextElement* moneyGainLabel;
  static ScrollingElement* endScroll;
  static Button* endButton;
  static Textbox* searchBox;
  static ScrollingElement* optionsScroll;
};
