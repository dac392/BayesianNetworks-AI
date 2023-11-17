#ifndef PROBABILITY_H
#define PROBABILITY_H


#include <vector>

#include "BayesianPairs.h"
#include "Belief.h"
class Probability
{

private:
    std::vector<std::vector<float>> probability_matrix;
    Belief belief_table;
    BayesianPairs table;
    int dimensions;

public:
    Probability(int dimensions);
    void init(const std::vector<std::pair<int, int>>& open, bool needsNetwork);
    bool positionIsOpen(int i , int j);
    std::vector<std::vector<float>> getProbabilities();
    std::vector<std::pair<int, int>> getHighestProbabilityList();
    void updateProbabilities(const std::pair<int, int>& pos, float norm);
    void normalizeProbabilities(float modifier);
    float getProbabilityAt(const std::pair<int, int>& pos);
    void scaleProbabilities(const std::pair<int, int>& pos, float modifier);
    void scaleBayesianPair(const std::pair<int,int>& x, const std::pair<int, int>& y, float mod);
    void normalizePairs(float factor);
    std::vector<std::pair<int, int>> getIntelligentStep(std::pair<int, int> current);
    void remove(const std::pair<int, int>& current);
    void smallerRemove(const std::pair<int, int>& current);
    float getPairProbabilityAt(const std::pair<int, int>& i, const std::pair<int, int>& j);
    void narrowDownSearchSpace(const std::pair<int, int>& current);
    void updatePairProbability(Table& distances, Sensor& sensor, const std::pair<int, int>& curr, bool signalDetected);
    void lateGameUpdate(Table& distances, Sensor& sensor, const std::pair<int, int>& curr, bool signalDetected);

    std::pair<int, int> getPreferedPosition(const std::pair<int, int>& position);
    void simpleHeuristicUpdate();
    void harderHeuristicUpdate();
};

#endif // PROBABILITY_H
