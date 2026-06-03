#include "Container.h"

Container::Container(std::vector<Object*> objects) : objects(objects) {}

void Container::changeVisibility(bool visible) {
  for (Object* object : objects) {
    object->visible = visible;
  }
}

void Container::registerObjects() {
  for (Object* object : objects) {
    object->registerObject();
  }
}
