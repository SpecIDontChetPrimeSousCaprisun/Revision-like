#include "Window.h"
#include "Object.h"
#include "Sound.h"
#include "UIElement.h"
#include "TextElement.h"
#include "Textbox.h"
#include "Button.h"
#include "Gameloop.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main() {
  Sound::init();

  if (Window::init() == -1) return -1;      
 
  Object::initShader();
  TextElement::initShader();
  Textbox::init();
  Gameloop::init();

  Window::mainLoop();
  Sound::unInit();
}
