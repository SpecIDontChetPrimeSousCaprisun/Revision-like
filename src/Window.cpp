#include "Window.h"
#include "Object.h"
#include "UIElement.h"
#include "Button.h"
#include "Container.h"
#include "Gameloop.h"
#include "MainMenu.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <sstream>
#include <iomanip>

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
  window = glfwCreateWindow(fbWidth, fbHeight, "Revision gaem", NULL, NULL);

  if (!window)
  {
      glfwTerminate();
      return -1;
  } 

  /* Make the window's context current */
  glfwMakeContextCurrent(window);
  glfwSwapInterval(0); 
  glfwSetWindowMonitor(window, 1 ? glfwGetPrimaryMonitor() : NULL, 0, 0, fbWidth, fbHeight, GLFW_DONT_CARE);


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

  MainMenu::update();
  Object::registerAll();
  Container::deletePendingObjects();
  Gameloop::stepFoward();
  Gameloop::update();
  Object::updateAll();
  Object::drawAll();

  glfwSwapBuffers(window);
}

void Window::mainLoop() {
  MainMenu::init();

  fpsLabel = new TextElement(glm::vec2(0.0f, 0.0f), glm::vec2(0.25f, 0.1f), 1.0f, "textures/Wallpaper.jpeg", 2, "FPS : 0", "fonts/Kenney Future Narrow.ttf", glm::vec3(0.0f, 0.0f, 0.0f));
  //fpsLabel->registerObject();

  while (!glfwWindowShouldClose(window) && inGame){
    updateFrame();
  }

  glfwTerminate();
}
