#include "FrenchRandom.h"

std::mt19937 FrenchRandom::rng(std::random_device{}());
std::vector<std::string> FrenchRandom::sentencesPool;
std::map<std::string, SentenceInfo*> FrenchRandom::sentencesInfo;

std::string FrenchRandom::getRandomSentence() {
  std::uniform_int_distribution<int> dist(0, sentencesPool.size() - 1);
  return sentencesPool[dist(rng)];
}

void FrenchRandom::init() {
  generateSentences();
  createPool();
}

void FrenchRandom::createPool() {
  sentencesPool.clear();

  for (auto& [sentence, info] : sentencesInfo) {
    sentencesPool.push_back(sentence);
  }
}

void FrenchRandom::generateSentences() {
  SentenceInfo* newInfo = new SentenceInfo();

  newInfo->verbs.push_back("mangent");
  newInfo->subjects.push_back("chevres");
  newInfo->CODs.push_back("l'herbe");

  sentencesInfo["Les chevres mangent l'herbe."] = newInfo;
}
