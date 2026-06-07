#include "Window.h"
#include "Object.h"
#include "UIElement.h"
#include "Button.h"
#include "Container.h"
#include "Gameloop.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <sstream>
#include <iomanip>

// REQUIRED definition (missing in your case)
GLFWwindow* Window::window = nullptr;
double Window::deltaTime = 0;
int Window::fbWidth = 600;
int Window::fbHeight = 480;
bool Window::inGame = true;
TextElement* Window::fpsLabel;
Container* Window::menu;
ma_sound* Window::music;

double lastFrame = 0.0;
bool toggledMenu = false;
bool resizing = false;

int Window::init() {
  glfwSetErrorCallback([](int error, const char* description) {
      std::cout << "GLFW ERROR: " << description << "\n";
  }); 

  /* Initialize the library */
  if (!glfwInit())
      return -1; 

  glfwWindowHint(GLFW_DEPTH_BITS, 24);

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(fbWidth, fbHeight, "Revise like", NULL, NULL);

  if (!window)
  {
      glfwTerminate();
      return -1;
  } 

  /* Make the window's context current */
  glfwMakeContextCurrent(window);
  glfwSwapInterval(0); 

  #ifndef __EMSCRIPTEN__
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
  #endif

  glClearColor(0.2f, 0.4f, 0.6f, 1.0f);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glm::mat4 projection = glm::ortho(
      0.0f, (float)fbWidth,
      (float)fbHeight, 0.0f,
      -1.0f, 1.0f
  );
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

  glfwSetFramebufferSizeCallback(window,
  [](GLFWwindow*, int width, int height)
  {
      Window::fbWidth = width;
      Window::fbHeight = height;

      resizing = (width == 0 || height == 0);

      glViewport(0, 0, width, height);
  });

  return 0;
}

void Window::updateFrame() {
  if (resizing) {
    glfwPollEvents();
    glfwSwapBuffers(window);
    return;
  }

  glfwPollEvents();

  double currentFrame = glfwGetTime();
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;

  double fps = 1.0 / deltaTime; 

  if (deltaTime > 0.1) {
    deltaTime = 0.1;
  } 

  std::ostringstream ss;
  ss << std::fixed << std::setprecision(1) << fps;
  std::string fpsString = ss.str();

  fpsLabel->text = "FPS : " + fpsString; 

  glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

  if (fbWidth == 0 || fbHeight == 0) {
    glfwSwapBuffers(window);
    return;
  }
 
  glViewport(0, 0, fbWidth, fbHeight);

  glClear(GL_COLOR_BUFFER_BIT);

  Object::registerAll();
  Container::deletePendingObjects();
  Gameloop::stepFoward();
  Gameloop::update();
  Object::updateAll();
  Object::drawAll();

  glfwSwapBuffers(window);
}

void Window::mainLoop() {
  std::vector<Object*> mainButtons;
  std::vector<Object*> subjectButtons;

  // Main menu

  UIElement* background = new UIElement(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), 0.0f, glm::vec3(0.512f, 0.0f, 0.750f), 0);
  TextElement* title = new TextElement(glm::vec2(0.5f, 0.05f), glm::vec2(1.0f, 0.15f), 1.0f, "textures/Wallpaper.jpeg", 1, "Revision like", "fonts/Kenney Future Narrow.ttf", glm::vec3(0.0f, 0.0f, 0.0f));
  Button* reviseButton = new Button(glm::vec2(0.5f, 0.45f), glm::vec2(0.5f, 0.1f), 0.0f, glm::vec3(0.3f, 0.0f, 0.3f), 1, "Reviser", "fonts/Kenney Future Narrow.ttf", glm::vec3(0.0f, 0.0f, 0.0f));
  Button* continueButton = new Button(glm::vec2(0.5f, 0.575f), glm::vec2(0.5f, 0.1f), 0.0f, glm::vec3(0.3f, 0.0f, 0.3f), 1, "Continuer", "fonts/Kenney Future Narrow.ttf", glm::vec3(0.0f, 0.0f, 0.0f));
  Button* creditsButton = new Button(glm::vec2(0.5f, 0.7f), glm::vec2(0.5f, 0.1f), 0.0f, glm::vec3(0.3f, 0.0f, 0.3f), 1, "Credits", "fonts/Kenney Future Narrow.ttf", glm::vec3(0.0f, 0.0f, 0.0f));
  Button* quitButton = new Button(glm::vec2(0.5f, 0.825f), glm::vec2(0.5f, 0.1f), 0.0f, glm::vec3(0.3f, 0.0f, 0.3f), 1, "Quitter", "fonts/Kenney Future Narrow.ttf", glm::vec3(1.0f, 0.0f, 0.0f));

  mainButtons.push_back(reviseButton);
  mainButtons.push_back(continueButton);
  mainButtons.push_back(creditsButton);
  mainButtons.push_back(quitButton);

  Container* mainButtonsContainer = new Container(mainButtons);

  title->anchorPoint = glm::vec2(0.5f, 0.0f);
  reviseButton->anchorPoint = glm::vec2(0.5f, 0.0f);
  continueButton->anchorPoint = glm::vec2(0.5f, 0.0f);
  creditsButton->anchorPoint = glm::vec2(0.5f, 0.0f);
  quitButton->anchorPoint = glm::vec2(0.5f, 0.0f);

  quitButton->setCallback([]() {
    inGame = false;
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

  Container* subjectButtonsContainer = new Container(subjectButtons);

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

  fpsLabel = new TextElement(glm::vec2(0.0f, 0.0f), glm::vec2(0.25f, 0.1f), 1.0f, "textures/Wallpaper.jpeg", 2, "FPS : 0", "fonts/Kenney Future Narrow.ttf", glm::vec3(0.0f, 0.0f, 0.0f));
  //fpsLabel->registerObject();

  while (!glfwWindowShouldClose(window) && inGame){
    updateFrame();
  }

  glfwTerminate();
}
