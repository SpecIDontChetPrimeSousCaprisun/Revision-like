#pragma once

#include <vector>
#include <string>
#include <random>
#include <functional>
#include <map>

class Professor {
public:
  static void init();
  static std::vector<Professor*> getRandomProfessors();
  static void updateAll(std::map<std::string, std::vector<std::string>> answers, std::string sentence);

  static std::mt19937 rng;
  static std::vector<Professor*> professors; 

  float price;
  std::string subject, name, texPath, description;
protected:
  using Factory = std::function<Professor*()>;

  static std::vector<Factory>& getRegistry() {
      static std::vector<Factory> registry;
      return registry;
  }

  Professor(float price, std::string subject, std::string name, std::string texPath, std::string description);

  virtual void intervene(std::map<std::string, std::vector<std::string>> answers, std::string sentence);
};
