#ifndef BELIEF_H
#define BELIEF_H

#include "Coordinate.h"
#include <vector>
class Belief
{
private:
    std::vector<float> belief;
public:
    Belief();
    int coordinateToIndex(Coordinate pos);
    std::pair<int, int> indexToCoordinate(int index);
    std::pair<int, int> indexToMainCoordinate(int index);
    void updateBelief(Coordinate pos, float prob);
    void resetBelief();
    std::pair<int, int> getMostFavorablePosition();
    std::pair<int, int> getFavoritePositions(const std::pair<int, int>& position);
    int coordinateToSubsectionIndex(const std::pair<int, int>& position);
    std::vector<int> getNeighborSubsections(const std::pair<int, int>& position);
};

#endif //BELIEF_H
