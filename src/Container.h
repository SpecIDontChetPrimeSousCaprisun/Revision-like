#pragma once

#include "Object.h"
#include <vector>

class Container {
public:
  Container(std::vector<Object*> objects);
  std::vector<Object*> objects;
  
  void changeVisibility(bool visible);
  void registerObjects();
};
