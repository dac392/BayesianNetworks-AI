#ifndef PROBABILITY_H
#define PROBABILITY_H


#include <vector>

#include "BayesianPairs.h"
class Probability
{

private:
    std::vector<std::vector<float>> probability_matrix;
    BayesianPairs table;
    int dimensions;
    int total_open_pairs;

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
    float getProbabilityAt(std::pair<int, int>& pos);
    void scaleProbabilities(std::pair<int, int>& pos, float modifier);
    void scaleBayesianPair(const std::pair<int,int>& x, const std::pair<int, int>& y, float mod);
    void normalizePairs(const std::vector<std::pair<int, int>>& open, float factor);
    std::vector<std::pair<int, int>> getIntelligentStep(std::pair<int, int> current);
    void remove(const std::pair<int, int>& current);
    //void normalizePairs(const std::vector<std::pair<int, int>>& open, float modifier);
    float getPairProbabilityAt(const std::pair<int, int>& i, const std::pair<int, int>& j);
    void narrowDownSearchSpace(const std::pair<int, int>& current);
};

#endif // PROBABILITY_H
