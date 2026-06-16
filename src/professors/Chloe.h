#pragma once

#include "Professor.h"

class Chloe : public Professor {
public:
  Chloe();

  static bool registered;
protected:
  void intervene(std::map<std::string, std::vector<std::string>> answers, std::string sentence) override;
};
