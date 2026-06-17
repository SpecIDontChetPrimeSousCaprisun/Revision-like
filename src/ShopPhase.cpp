#include "ShopPhase.h"
#include "FrenchRandom.h"
#include "Gameloop.h"
#include "UIElement.h"
#include "Window.h"
#include "professors/Professor.h"
#include "professors/Chloe.h"
#include <vector>
#include <fstream>
#include <iomanip>

TextElement* ShopPhase::moneyText;
Container* ShopPhase::UI;
std::vector<Container*> ShopPhase::upgradeButtons;

void ShopPhase::init() {
  std::vector<Object*> UIElements;

  UIElement* background = new UIElement(glm::vec2(0.5f, 0.5f), glm::vec2(0.4f, 0.9f), 0.0f, glm::vec3(0.4f, 0.0f, 0.4f), 2);
  TextElement* title = new TextElement(glm::vec2(0.5f, 0.05f), glm::vec2(0.35f, 0.1f), 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), 3, "SHOP", "fonts/Kenney Future Narrow.ttf", glm::vec3(1.0f, 1.0f, 1.0f));
  UIElement* expressionsContainer = new UIElement(glm::vec2(0.5f, 0.375f), glm::vec2(0.35f, 0.15f), 0.0f, glm::vec3(0.512f, 0.0f, 0.512f), 3);
  UIElement* professorsContainer = new UIElement(glm::vec2(0.5f, 0.15f), glm::vec2(0.35f, 0.2f), 0.0f, glm::vec3(0.512f, 0.0f, 0.512f), 3);
  moneyText = new TextElement(glm::vec2(0.01f, 1.0f), glm::vec2(1.0f, 0.1f), 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), 2, "0$", "fonts/Kenney Future Narrow.ttf", glm::vec3(0.0f, 1.0f, 0.0f));
  Button* endButton = new Button(glm::vec2(0.5f, 0.94f), glm::vec2(0.35f, 0.05f), 0.0f, glm::vec3(0.512f, 0.0f, 0.512f), 2, "Continue", "fonts/Kenney Future Narrow.ttf", glm::vec3(1.0f, 1.0f, 1.0f));

  background->anchorPoint = glm::vec2(0.5f, 0.5f);

  title->anchorPoint = glm::vec2(0.5f, 0.0f);

  expressionsContainer->anchorPoint = glm::vec2(0.5f, 0.0f);

  professorsContainer->anchorPoint = glm::vec2(0.5f, 0.0f);

  moneyText->anchorPoint = glm::vec2(0.0f, 1.0f);
  moneyText->textCentered = false;

  endButton->anchorPoint = glm::vec2(0.5f, 1.0f);
  endButton->setCallback([]() {
    end();
  });

  UIElements.push_back(background);
  UIElements.push_back(title);
  UIElements.push_back(expressionsContainer);
  UIElements.push_back(professorsContainer);
  UIElements.push_back(moneyText);
  UIElements.push_back(endButton);

  UI = new Container(UIElements);
  UI->changeVisibility(false);
  UI->changeCornerRadius(0.025f);
  UI->registerObjects();
}

ShopUpgrade* ShopPhase::addNewUpgrade(
  int i, 
  float padding, 
  size_t numberOfUpgrades, 
  glm::vec2 position,
  glm::vec2 size, std::string texPath, 
  std::string upgrade, 
  float price,
  std::string desc
) {
  std::ostringstream pricess;
  pricess << std::fixed << std::setprecision(1) << price;

  size.x /= numberOfUpgrades;
  size.x -= padding;
  position.x += padding / 2;
  position.x += (size.x + padding) * i;

  std::vector<Object*> objects;

  Button* upgradeButton = new Button(position, size, 0.0f, texPath, 4, "", "fonts/Kenney Future Narrow.ttf", glm::vec3(1.0f, 1.0f, 1.0f));
  TextElement* upgradeTitle = new TextElement(position, glm::vec2(size.x, 0.025f), 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), 5, upgrade, "fonts/Kenney Future Narrow.ttf", glm::vec3(1.0f, 1.0f, 1.0f));
  TextElement* priceElement = new TextElement(position + glm::vec2(0.0f, size.y), glm::vec2(size.x, 0.025f), 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), 5, pricess.str() + "$", "fonts/Kenney Future Narrow.ttf", glm::vec3(0.0f, 1.0f, 0.0f));
  TextElement* boughtElement = new TextElement(position + glm::vec2(0.0f, size.y / 2), glm::vec2(size.x, 0.025f), 0.25f, glm::vec3(0.0f, 0.0f, 0.0f), 5, "SOLD", "fonts/Kenney Future Narrow.ttf", glm::vec3(0.0f, 1.0f, 0.0f));
  TextElement* descElement = new TextElement(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.025f), 0.0f, glm::vec3(0.3f, 0.0f, 0.3f), 6, desc, "fonts/Kenney Future Narrow.ttf", glm::vec3(1.0f, 1.0f, 1.0f));

  descElement->textCentered = false;
  descElement->recalculateTextWidth();
  descElement->size.x = descElement->textWidth / Window::fbWidth;
  descElement->visible = false;
  descElement->registerObject();

  upgradeTitle->recalculateTextWidth();

  while (upgradeTitle->textWidth / Window::fbWidth > size.x) {
    upgradeTitle->size.y -= 0.001f;
    upgradeTitle->reloadFont("fonts/Kenney Future Narrow.ttf");
    upgradeTitle->recalculateTextWidth();
  }

  upgradeButton->cornerRadius = 0.025f;
  upgradeButton->setHoverCallback([descElement](bool hovered) {
    descElement->visible = hovered;
  });

  priceElement->anchorPoint = glm::vec2(0.0f, 1.0f);

  boughtElement->visible = false;
  boughtElement->anchorPoint = glm::vec2(0.0f, 0.5f);

  objects.push_back(upgradeButton);
  objects.push_back(upgradeTitle);
  objects.push_back(priceElement);
  objects.push_back(boughtElement);

  Container* upgradeContainer = new Container(objects);
  upgradeContainer->registerObjects();
  upgradeButtons.push_back(upgradeContainer);

  ShopUpgrade* shopUpgrade = new ShopUpgrade();

  shopUpgrade->upgradeButton = upgradeButton;
  shopUpgrade->boughtElement = boughtElement;

  return shopUpgrade;
}

void ShopPhase::start() {
  UI->changeVisibility(true);

  int i = 0;
  float padding = 0.01f;
  std::vector<UpgradeInfo*> upgrades = FrenchRandom::getRandomUpgrades();

  for (UpgradeInfo* upgrade : upgrades) {
    glm::vec2 position(0.325f, 0.3875f);
    glm::vec2 size(0.35f, 0.125f);

    ShopUpgrade* shopUpgrade = addNewUpgrade(
      i, 
      padding, 
      upgrades.size(), 
      position, size, 
      upgrade->texPath, 
      upgrade->upgrade, 
      upgrade->price,
      ""
    );

    shopUpgrade->upgradeButton->setCallback([upgrade, shopUpgrade]() {
      if (upgrade->price > Gameloop::money) return;

      Gameloop::money -= upgrade->price;
      Gameloop::upgrades.push_back(upgrade->upgrade);
      shopUpgrade->upgradeButton->interactible = false;
      shopUpgrade->boughtElement->visible = true;
    });

    i++;
  }

  i = 0;
  std::vector<Professor*> professors = Professor::getRandomProfessors();

  for (Professor* professor : professors) {
    glm::vec2 position(0.325f, 0.1625f);
    glm::vec2 size(0.35f, 0.175f);

    ShopUpgrade* shopUpgrade = addNewUpgrade(
      i, 
      padding, 
      professors.size(), 
      position, 
      size, 
      professor->texPath, 
      professor->name, 
      professor->price,
      professor->description
    );

    shopUpgrade->upgradeButton->setCallback([professor, shopUpgrade]() {
      if (professor->price > Gameloop::money) return;

      Gameloop::money -= professor->price;
      Gameloop::professors.push_back(professor);
      shopUpgrade->upgradeButton->interactible = false;
      shopUpgrade->boughtElement->visible = true;
    });

    i++;
  }
}

void ShopPhase::update() {
  std::ostringstream moneyss;
  moneyss << std::fixed << std::setprecision(1) << Gameloop::money;
  moneyText->text = moneyss.str() + "$";
}

void ShopPhase::end() {
  Gameloop::completedStage = true;
  UI->changeVisibility(false);

  for (Container* buttons : upgradeButtons) {
    buttons->clear();
    buttons->pendDelete();
  }

  upgradeButtons.clear();
}
