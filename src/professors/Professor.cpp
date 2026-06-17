#include "Professor.h"
#include "../Gameloop.h"
#include <algorithm>

std::mt19937 Professor::rng(std::random_device{}());
std::vector<Professor*> Professor::professors;

void Professor::init() {
  for (auto& factory : getRegistry()) {
    factory();
  }
}

void Professor::updateAll(std::map<std::string, std::vector<std::string>> answers, std::string sentence) {
  float i = 0;
  float padding = 0.01f;
  glm::vec2 pos(0.5f, 0.025f);
  pos.x -= (0.05f + padding) * (Gameloop::professors.size() - 1);

  for (Professor* professor : Gameloop::professors) {
    professor->element->visible = true;
    professor->element->position = glm::vec2(pos.x + ((professor->element->size.x + padding) * i), pos.y);
    professor->intervene(answers, sentence);
    i++;
  }
}

void Professor::hideAll() {
  for (Professor* professor : Gameloop::professors) {
    professor->element->visible = false;
  }
}

Professor::Professor(float price, std::string subject, std::string name, std::string texPath, std::string description) 
  : price(price), subject(subject), name(name), texPath(texPath), description(description), 
    element(new UIElement(glm::vec2(0.0f, 0.0f), glm::vec2(0.1f, 0.275f), 0.0f, texPath, 30)) {
  element->anchorPoint = glm::vec2(0.5f, 0.0f);
  element->visible = false;
  element->registerObject();
  professors.push_back(this);
}

std::vector<Professor*> Professor::getRandomProfessors() {
  std::vector<Professor*> toRemove;

  for (Professor* professor : professors) {
    if (std::find(Gameloop::professors.begin(), Gameloop::professors.end(), professor) != Gameloop::professors.end()) {
      toRemove.push_back(professor);
    }
  }

  for (Professor* professor : toRemove) {
    professors.erase(
      std::remove(professors.begin(), professors.end(), professor),
      professors.end()
    );
  }

  std::vector<Professor*> currentPool = professors;
  std::vector<Professor*> result;

  for (int i = 0; i < 3; i++) {
    if (currentPool.empty()) break;

    std::uniform_int_distribution<int> dist(0, currentPool.size() - 1);
    int index = dist(rng);

    result.push_back(currentPool[index]);
    currentPool.erase(currentPool.begin() + index);
  }

  return result;
}

void Professor::intervene(std::map<std::string, std::vector<std::string>> answers, std::string sentence) {}


