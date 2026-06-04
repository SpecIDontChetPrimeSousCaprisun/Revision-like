#include "Button.h"
#include "Window.h"

Button::Button(glm::vec2 position, glm::vec2 size, float transparency, std::string texPath, int zIndex, std::string text, std::string fontPath, glm::vec3 textColor) 
  : TextElement(position, size, transparency, texPath, zIndex, text, fontPath, textColor), pressed(false), ignoreCurrentInput(false) {
  setCallback([]() {
    std::cout << "pressed\n";
  });
}

Button::Button(glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 color, int zIndex, std::string text, std::string fontPath, glm::vec3 textColor) 
  : TextElement(position, size, transparency, color, zIndex, text, fontPath, textColor), pressed(false), ignoreCurrentInput(false) {
  setCallback([]() {
    std::cout << "pressed\n";
  });
}

void Button::setCallback(std::function<void()> callback) {
  this->callback = callback;
}

void Button::beforeUpdate() {
  if ((!visible ||
       ignoreCurrentInput) &&
      glfwGetMouseButton(Window::window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
    ignoreCurrentInput = true;
  } else {
    ignoreCurrentInput = false;
  }
}

drawInfo* Button::beforeDrawing(drawInfo* info) {
  double mouseX;
  double mouseY;

  glfwGetCursorPos(Window::window, &mouseX, &mouseY);

  drawInfo* newInfo = baseBeforeDrawing(info);
  
  if (mouseX >= newInfo->position.x &&
      mouseX <= newInfo->position.x + newInfo->size.x &&
      mouseY >= newInfo->position.y &&
      mouseY <= newInfo->position.y + newInfo->size.y
      && !ignoreCurrentInput) {
    if (glfwGetMouseButton(Window::window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
      colorChange = glm::vec3(0.1f, 0.1f, 0.1f);
      if (!pressed) {
        pressed = true;
        callback();
      }
    }
    else {
      colorChange = glm::vec3(-0.1f, -0.1f, -0.1f);
      pressed = false;
    }
  } else {
    pressed = false;
    colorChange = glm::vec3(0.0f, 0.0f, 0.0f);
  }

  return newInfo;
}
