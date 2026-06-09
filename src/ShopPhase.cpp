#include "ShopPhase.h"
#include "FrenchRandom.h"
#include "Gameloop.h"
#include "UIElement.h"
#include "Window.h"
#include <vector>
#include <fstream>
#include <iomanip>

TextElement* ShopPhase::moneyText;
Container* ShopPhase::UI;
std::vector<Container*> ShopPhase::upgradeButtons;

void ShopPhase::init() {
  std::vector<Object*> UIElements;

  UIElement* background = new UIElement(glm::vec2(0.5f, 0.5f), glm::vec2(0.75f, 0.9f), 0.0f, glm::vec3(0.4f, 0.0f, 0.4f), 2);
  TextElement* title = new TextElement(glm::vec2(0.5f, 0.05f), glm::vec2(0.75f, 0.1f), 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), 3, "SHOP", "fonts/Kenney Future Narrow.ttf", glm::vec3(1.0f, 1.0f, 1.0f));
  UIElement* expressionsContainer = new UIElement(glm::vec2(0.5f, 0.15f), glm::vec2(0.7f, 0.2f), 0.0f, glm::vec3(0.512f, 0.0f, 0.512f), 3);
  moneyText = new TextElement(glm::vec2(0.01f, 1.0f), glm::vec2(1.0f, 0.1f), 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), 2, "0$", "fonts/Kenney Future Narrow.ttf", glm::vec3(0.0f, 1.0f, 0.0f));
  Button* endButton = new Button(glm::vec2(0.5f, 0.94f), glm::vec2(0.7f, 0.05f), 0.0f, glm::vec3(0.512f, 0.0f, 0.512f), 2, "Continue", "fonts/Kenney Future Narrow.ttf", glm::vec3(1.0f, 1.0f, 1.0f));

  background->anchorPoint = glm::vec2(0.5f, 0.5f);

  title->anchorPoint = glm::vec2(0.5f, 0.0f);

  expressionsContainer->anchorPoint = glm::vec2(0.5f, 0.0f);

  moneyText->anchorPoint = glm::vec2(0.0f, 1.0f);
  moneyText->textCentered = false;

  endButton->anchorPoint = glm::vec2(0.5f, 1.0f);
  endButton->setCallback([]() {
    end();
  });

  UIElements.push_back(background);
  UIElements.push_back(title);
  UIElements.push_back(expressionsContainer);
  UIElements.push_back(moneyText);
  UIElements.push_back(endButton);

  UI = new Container(UIElements);
  UI->changeVisibility(false);
  UI->changeCornerRadius(0.025f);
  UI->registerObjects();
}

void ShopPhase::start() {
  UI->changeVisibility(true);

  int i = 0;
  float padding = 0.01f;
  std::vector<UpgradeInfo*> upgrades = FrenchRandom::getRandomUpgrades();

  for (UpgradeInfo* upgrade : upgrades) {
    std::ostringstream pricess;
    pricess << std::fixed << std::setprecision(1) << upgrade->price;
  
    glm::vec2 position(0.15f, 0.1625f);
    glm::vec2 size((0.7f / upgrades.size()) - (padding), 0.175f);

    position.x += padding / 2;
    position.x += (size.x + padding) * i;

    std::vector<Object*> objects;

    Button* upgradeButton = new Button(position, size, 0.0f, upgrade->texPath, 4, "", "fonts/Kenney Future Narrow.ttf", glm::vec3(1.0f, 1.0f, 1.0f));
    TextElement* upgradeTitle = new TextElement(position, glm::vec2(size.x, 0.025f), 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), 5, upgrade->upgrade, "fonts/Kenney Future Narrow.ttf", glm::vec3(1.0f, 1.0f, 1.0f));
    TextElement* priceElement = new TextElement(position + glm::vec2(0.0f, size.y), glm::vec2(size.x, 0.025f), 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), 5, pricess.str() + "$", "fonts/Kenney Future Narrow.ttf", glm::vec3(0.0f, 1.0f, 0.0f));
    TextElement* boughtElement = new TextElement(position + glm::vec2(0.0f, size.y / 2), glm::vec2(size.x, 0.025f), 0.25f, glm::vec3(0.0f, 0.0f, 0.0f), 5, "SOLD", "fonts/Kenney Future Narrow.ttf", glm::vec3(0.0f, 1.0f, 0.0f));

    upgradeTitle->recalculateTextWidth();

    while (upgradeTitle->textWidth / Window::fbWidth > size.x) {
      upgradeTitle->size.y -= 0.001f;
      upgradeTitle->reloadFont("fonts/Kenney Future Narrow.ttf");
      upgradeTitle->recalculateTextWidth();
    }

    upgradeButton->cornerRadius = 0.025f;
    upgradeButton->setCallback([upgrade, upgradeButton, boughtElement]() {
      if (upgrade->price > Gameloop::money) return;

      Gameloop::money -= upgrade->price;
      Gameloop::upgrades.push_back(upgrade->upgrade);
      upgradeButton->interactible = false;
      boughtElement->visible = true;
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
