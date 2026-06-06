#pragma once

#include <string>
#include <vector>
#include <map>
#include <random>

class SentenceInfo {
public:
  std::map<std::string, std::vector<std::string>> types;
};

class FrenchRandom {
public:
  static void init();
  static void evaluatePoints(std::map<std::string, std::vector<std::string>> answers, std::string sentence);
  static std::string getRandomSentence();
private:
  static void generateSentences();
  static void createPool();

  static std::mt19937 rng;
  static std::vector<std::string> sentencesPool;
  static std::map<std::string, SentenceInfo*> sentencesInfo;
};
