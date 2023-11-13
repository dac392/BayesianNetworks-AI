#ifndef BAYESIANPAIRS_H
#define BAYESIANPAIRS_H

#include <list>
#include <utility>
#include <unordered_map>
#include <vector>

#include "Coordinate.h"



class BayesianPairs
{
private:
    std::unordered_map<std::pair<Coordinate, Coordinate>, float, distance_hash> hash_map;
    int size;

public:
    BayesianPairs();
    void addPair(const std::pair<int, int>& c1, const std::pair<int, int>& c2);
    void setProbability(const std::pair<int, int>& c1, const std::pair<int, int>& c2, float modifier);


    void scalePairs(const std::pair<int,int>& x, const std::pair<int, int>& y, float mod);
    void normalize(const std::pair<int, int>& i, const std::pair<int, int>& j, float factor);
    std::vector<std::pair<int, int>> getHighestProbabilities(const std::pair<int, int>& current);
    void remove(const std::pair<int, int>& current);
    float get(const std::pair<int, int>& i, const std::pair<int, int>& j);
    void redistribute(float modifier);
    void narrowDownSearchSpace(const std::pair<int, int>& current);
};

#endif // BAYESIANPAIRS_H