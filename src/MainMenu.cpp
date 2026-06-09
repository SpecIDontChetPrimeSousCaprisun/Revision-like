#include "MainMenu.h"
#include "Button.h"
#include "Gameloop.h"
#include "Window.h"
#include <cmath>

float MainMenu::titleSpeed = 2.0f;
TextElement* MainMenu::title;
Container* MainMenu::mainButtonsContainer;
Container* MainMenu::subjectButtonsContainer;

void MainMenu::init() {
  std::vector<Object*> mainButtons;
  std::vector<Object*> subjectButtons;

  // Main menu

  UIElement* background = new UIElement(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), 0.0f, glm::vec3(0.512f, 0.0f, 0.750f), 0);
  title = new TextElement(glm::vec2(0.5f, 0.05f), glm::vec2(1.0f, 0.15f), 1.0f, "textures/Wallpaper.jpeg", 1, "Revision like", "fonts/Kenney Future Narrow.ttf", glm::vec3(0.0f, 0.0f, 0.0f));
  Button* reviseButton = new Button(glm::vec2(0.5f, 0.45f), glm::vec2(0.5f, 0.1f), 0.0f, glm::vec3(0.3f, 0.0f, 0.3f), 2, "Reviser", "fonts/Kenney Future Narrow.ttf", glm::vec3(0.0f, 0.0f, 0.0f));
  UIElement* reviseButtonBackground = new UIElement(glm::vec2(0.5f, 0.445f), glm::vec2(0.51f, 0.11f), 0.0f, glm::vec3(0.0f, 0.0f, 0.0f), 1);
  Button* continueButton = new Button(glm::vec2(0.5f, 0.575f), glm::vec2(0.5f, 0.1f), 0.0f, glm::vec3(0.3f, 0.0f, 0.3f), 2, "Continuer", "fonts/Kenney Future Narrow.ttf", glm::vec3(0.0f, 0.0f, 0.0f));
  UIElement* continueButtonBackground = new UIElement(glm::vec2(0.5f, 0.57f), glm::vec2(0.51f, 0.11f), 0.0f, glm::vec3(0.0f, 0.0f, 0.0f), 1);
  Button* creditsButton = new Button(glm::vec2(0.5f, 0.7f), glm::vec2(0.5f, 0.1f), 0.0f, glm::vec3(0.3f, 0.0f, 0.3f), 2, "Credits", "fonts/Kenney Future Narrow.ttf", glm::vec3(0.0f, 0.0f, 0.0f));
  UIElement* creditsButtonBackground = new UIElement(glm::vec2(0.5f, 0.695f), glm::vec2(0.51f, 0.11f), 0.0f, glm::vec3(0.0f, 0.0f, 0.0f), 1);
  Button* quitButton = new Button(glm::vec2(0.5f, 0.825f), glm::vec2(0.5f, 0.1f), 0.0f, glm::vec3(0.3f, 0.0f, 0.3f), 2, "Quitter", "fonts/Kenney Future Narrow.ttf", glm::vec3(1.0f, 0.0f, 0.0f));
  UIElement* quitButtonBackground = new UIElement(glm::vec2(0.5f, 0.82f), glm::vec2(0.51f, 0.11f), 0.0f, glm::vec3(0.0f, 0.0f, 0.0f), 1);

  reviseButtonBackground->anchorPoint = glm::vec2(0.5f, 0.0f);
  reviseButtonBackground->visible = false;
  reviseButtonBackground->cornerRadius = 0.025f;
  reviseButtonBackground->registerObject();

  continueButtonBackground->anchorPoint = glm::vec2(0.5f, 0.0f);
  continueButtonBackground->visible = false;
  continueButtonBackground->cornerRadius = 0.025f;
  continueButtonBackground->registerObject();

  creditsButtonBackground->anchorPoint = glm::vec2(0.5f, 0.0f);
  creditsButtonBackground->visible = false;
  creditsButtonBackground->cornerRadius = 0.025f;
  creditsButtonBackground->registerObject();

  quitButtonBackground->anchorPoint = glm::vec2(0.5f, 0.0f);
  quitButtonBackground->visible = false;
  quitButtonBackground->cornerRadius = 0.025f;
  quitButtonBackground->registerObject();

  mainButtons.push_back(reviseButton);
  mainButtons.push_back(continueButton);
  mainButtons.push_back(creditsButton);
  mainButtons.push_back(quitButton);

  mainButtonsContainer = new Container(mainButtons);

  title->anchorPoint = glm::vec2(0.5f, 0.0f);
  reviseButton->anchorPoint = glm::vec2(0.5f, 0.0f);
  continueButton->anchorPoint = glm::vec2(0.5f, 0.0f);
  creditsButton->anchorPoint = glm::vec2(0.5f, 0.0f);
  quitButton->anchorPoint = glm::vec2(0.5f, 0.0f);

  quitButton->setCallback([]() {
      Window::inGame = false;
  });

  background->registerObject();
  title->registerObject();
  mainButtonsContainer->changeCornerRadius(0.025f);
  mainButtonsContainer->registerObjects(); 

  reviseButton->setCallback([mainButtonsContainer, subjectButtonsContainer]() {
    mainButtonsContainer->changeVisibility(false);
    subjectButtonsContainer->changeVisibility(true);
  });

  reviseButton->setHoverCallback([reviseButtonBackground](bool hovered) {
    reviseButtonBackground->visible = hovered;
  });

  continueButton->setHoverCallback([continueButtonBackground](bool hovered) {
    continueButtonBackground->visible = hovered;
  });

  creditsButton->setHoverCallback([creditsButtonBackground](bool hovered) {
    creditsButtonBackground->visible = hovered;
  });

  quitButton->setHoverCallback([quitButtonBackground](bool hovered) {
    quitButtonBackground->visible = hovered;
  });

  // Subject menu

  Button* frenchButton = new Button(glm::vec2(0.5f, 0.35f), glm::vec2(0.5f, 0.1f), 0.0f, glm::vec3(0.3f, 0.0f, 0.3f), 1, "Francais", "fonts/Kenney Future Narrow.ttf", glm::vec3(0.0f, 0.0f, 0.0f));
  UIElement* frenchButtonBackground = new UIElement(glm::vec2(0.5f, 0.345f), glm::vec2(0.51f, 0.11f), 0.0f, glm::vec3(0.0f, 0.0f, 0.0f), 1);
  Button* mathButton = new Button(glm::vec2(0.5f, 0.475f), glm::vec2(0.5f, 0.1f), 0.0f, glm::vec3(0.3f, 0.0f, 0.15f), 1, "Maths", "fonts/Kenney Future Narrow.ttf", glm::vec3(0.0f, 0.0f, 0.0f));
  UIElement* mathButtonBackground = new UIElement(glm::vec2(0.5f, 0.47f), glm::vec2(0.51f, 0.11f), 0.0f, glm::vec3(0.0f, 0.0f, 0.0f), 1);
  Button* englishButton = new Button(glm::vec2(0.5f, 0.6f), glm::vec2(0.5f, 0.1f), 0.0f, glm::vec3(0.3f, 0.3f, 0.0f), 1, "Anglais", "fonts/Kenney Future Narrow.ttf", glm::vec3(0.0f, 0.0f, 0.0f));
  UIElement* englishButtonBackground = new UIElement(glm::vec2(0.5f, 0.595f), glm::vec2(0.51f, 0.11f), 0.0f, glm::vec3(0.0f, 0.0f, 0.0f), 1);
  Button* geographyButton = new Button(glm::vec2(0.5f, 0.725f), glm::vec2(0.5f, 0.1f), 0.0f, glm::vec3(0.3f, 0.0f, 0.0f), 1, "Histoire / Geo / EMC", "fonts/Kenney Future Narrow.ttf", glm::vec3(0.0f, 0.0f, 0.0f));
  UIElement* geographyButtonBackground = new UIElement(glm::vec2(0.5f, 0.72f), glm::vec2(0.51f, 0.11f), 0.0f, glm::vec3(0.0f, 0.0f, 0.0f), 1);
  Button* backButton = new Button(glm::vec2(0.5f, 0.85f), glm::vec2(0.5f, 0.1f), 0.0f, glm::vec3(0.3f, 0.0f, 0.0f), 1, "Retour", "fonts/Kenney Future Narrow.ttf", glm::vec3(0.0f, 0.0f, 0.0f));
  UIElement* backButtonBackground = new UIElement(glm::vec2(0.5f, 0.845f), glm::vec2(0.51f, 0.11f), 0.0f, glm::vec3(0.0f, 0.0f, 0.0f), 1);

  frenchButtonBackground->anchorPoint = glm::vec2(0.5f, 0.0f);
  frenchButtonBackground->visible = false;
  frenchButtonBackground->cornerRadius = 0.025f;
  frenchButtonBackground->registerObject();

  mathButtonBackground->anchorPoint = glm::vec2(0.5f, 0.0f);
  mathButtonBackground->visible = false;
  mathButtonBackground->cornerRadius = 0.025f;
  mathButtonBackground->registerObject();

  englishButtonBackground->anchorPoint = glm::vec2(0.5f, 0.0f);
  englishButtonBackground->visible = false;
  englishButtonBackground->cornerRadius = 0.025f;
  englishButtonBackground->registerObject();

  geographyButtonBackground->anchorPoint = glm::vec2(0.5f, 0.0f);
  geographyButtonBackground->visible = false;
  geographyButtonBackground->cornerRadius = 0.025f;
  geographyButtonBackground->registerObject();

  backButtonBackground->anchorPoint = glm::vec2(0.5f, 0.0f);
  backButtonBackground->visible = false;
  backButtonBackground->cornerRadius = 0.025f;
  backButtonBackground->registerObject();

  subjectButtons.push_back(frenchButton);
  subjectButtons.push_back(mathButton);
  subjectButtons.push_back(englishButton);
  subjectButtons.push_back(geographyButton);
  subjectButtons.push_back(backButton);

  subjectButtonsContainer = new Container(subjectButtons);

  frenchButton->anchorPoint = glm::vec2(0.5f, 0.0f);
  mathButton->anchorPoint = glm::vec2(0.5f, 0.0f);
  englishButton->anchorPoint = glm::vec2(0.5f, 0.0f);
  geographyButton->anchorPoint = glm::vec2(0.5f, 0.0f);
  backButton->anchorPoint = glm::vec2(0.5f, 0.0f);

  frenchButton->setCallback([subjectButtonsContainer, title]() {
    Gameloop::looping = true;
    Gameloop::currentSubject = "French";
    subjectButtonsContainer->changeVisibility(false);
    title->visible = false;
  });

  subjectButtonsContainer->registerObjects();
  subjectButtonsContainer->changeCornerRadius(0.025f);
  subjectButtonsContainer->changeVisibility(false);

  backButton->setCallback([mainButtonsContainer, subjectButtonsContainer]() {
    subjectButtonsContainer->changeVisibility(false);
    mainButtonsContainer->changeVisibility(true);
  });

  frenchButton->setHoverCallback([frenchButtonBackground](bool hovered) {
    frenchButtonBackground->visible = hovered;
  });

  mathButton->setHoverCallback([mathButtonBackground](bool hovered) {
    mathButtonBackground->visible = hovered;
  });

  englishButton->setHoverCallback([englishButtonBackground](bool hovered) {
    englishButtonBackground->visible = hovered;
  });

  geographyButton->setHoverCallback([geographyButtonBackground](bool hovered) {
    geographyButtonBackground->visible = hovered;
  });

  backButton->setHoverCallback([backButtonBackground](bool hovered) {
    backButtonBackground->visible = hovered;
  });
}

void MainMenu::update() {
  title->rotation += Window::deltaTime * titleSpeed;

  if (title->rotation >= 2.5f) {
    titleSpeed = -std::abs(titleSpeed);
  } else if (title->rotation <= -2.5f) {
    titleSpeed = std::abs(titleSpeed);
  }
}
