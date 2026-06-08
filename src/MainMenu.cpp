#include "MainMenu.h"
#include "Button.h"
#include "Gameloop.h"
#include "Window.h"

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
  Button* reviseButton = new Button(glm::vec2(0.5f, 0.45f), glm::vec2(0.5f, 0.1f), 0.0f, glm::vec3(0.3f, 0.0f, 0.3f), 1, "Reviser", "fonts/Kenney Future Narrow.ttf", glm::vec3(0.0f, 0.0f, 0.0f));
  Button* continueButton = new Button(glm::vec2(0.5f, 0.575f), glm::vec2(0.5f, 0.1f), 0.0f, glm::vec3(0.3f, 0.0f, 0.3f), 1, "Continuer", "fonts/Kenney Future Narrow.ttf", glm::vec3(0.0f, 0.0f, 0.0f));
  Button* creditsButton = new Button(glm::vec2(0.5f, 0.7f), glm::vec2(0.5f, 0.1f), 0.0f, glm::vec3(0.3f, 0.0f, 0.3f), 1, "Credits", "fonts/Kenney Future Narrow.ttf", glm::vec3(0.0f, 0.0f, 0.0f));
  Button* quitButton = new Button(glm::vec2(0.5f, 0.825f), glm::vec2(0.5f, 0.1f), 0.0f, glm::vec3(0.3f, 0.0f, 0.3f), 1, "Quitter", "fonts/Kenney Future Narrow.ttf", glm::vec3(1.0f, 0.0f, 0.0f));

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
  mainButtonsContainer->registerObjects(); 

  // Subject menu

  Button* frenchButton = new Button(glm::vec2(0.5f, 0.35f), glm::vec2(0.5f, 0.1f), 0.0f, glm::vec3(0.3f, 0.0f, 0.3f), 1, "Francais", "fonts/Kenney Future Narrow.ttf", glm::vec3(0.0f, 0.0f, 0.0f));
  Button* mathButton = new Button(glm::vec2(0.5f, 0.475f), glm::vec2(0.5f, 0.1f), 0.0f, glm::vec3(0.3f, 0.0f, 0.15f), 1, "Maths", "fonts/Kenney Future Narrow.ttf", glm::vec3(0.0f, 0.0f, 0.0f));
  Button* englishButton = new Button(glm::vec2(0.5f, 0.6f), glm::vec2(0.5f, 0.1f), 0.0f, glm::vec3(0.3f, 0.3f, 0.0f), 1, "Anglais", "fonts/Kenney Future Narrow.ttf", glm::vec3(0.0f, 0.0f, 0.0f));
  Button* geographyButton = new Button(glm::vec2(0.5f, 0.725f), glm::vec2(0.5f, 0.1f), 0.0f, glm::vec3(0.3f, 0.0f, 0.0f), 1, "Histoire / Geo / EMC", "fonts/Kenney Future Narrow.ttf", glm::vec3(0.0f, 0.0f, 0.0f));
  Button* backButton = new Button(glm::vec2(0.5f, 0.85f), glm::vec2(0.5f, 0.1f), 0.0f, glm::vec3(0.3f, 0.0f, 0.0f), 1, "Retour", "fonts/Kenney Future Narrow.ttf", glm::vec3(0.0f, 0.0f, 0.0f));

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
  subjectButtonsContainer->changeVisibility(false);

  backButton->setCallback([mainButtonsContainer, subjectButtonsContainer]() {
    subjectButtonsContainer->changeVisibility(false);
    mainButtonsContainer->changeVisibility(true);
  });

  reviseButton->setCallback([mainButtonsContainer, subjectButtonsContainer]() {
    mainButtonsContainer->changeVisibility(false);
    subjectButtonsContainer->changeVisibility(true);
  });
}

void MainMenu::update() {
  title->rotation += Window::deltaTime * titleSpeed;

  if (title->rotation >= 2.5f || title->rotation <= -2.5f) {
    titleSpeed = -titleSpeed;
  }
}
