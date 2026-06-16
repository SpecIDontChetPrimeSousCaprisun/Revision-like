#pragma once

#include <string>
#include <vector>
#include <map>
#include <random>

class SentenceInfo {
public:
  std::map<std::string, std::vector<std::string>> types;
  std::vector<std::string> requirements;
};

class UpgradeInfo {
public:
  float price;
  std::string upgrade, texPath;
};

class FrenchRandom {
public:
  static std::mt19937 rng;
  static std::vector<UpgradeInfo*> upgradePool;
  static std::vector<std::string> sentencesPool;
  static std::map<std::string, SentenceInfo*> sentencesInfo;
  static float timeToWait, timeToAdd;

  static void init();
  static void evaluatePoints(std::map<std::string, std::vector<std::string>> answers, std::string sentence);
  static std::string getRandomSentence();
  static std::vector<UpgradeInfo*> getRandomUpgrades();
private:
  static void generateSentences();
  static void createPool();
  static void createUpgradePool(); 
};
