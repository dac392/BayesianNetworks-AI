#include "../headers/Belief.h"
#include "../headers/Utility.h"
#include <limits>

#define ELEMENTS 25

Belief::Belief()
{
    belief = std::vector<float>(ELEMENTS, 0.0f);
}

int Belief::coordinateToIndex(Coordinate pos) {
    int row = pos.y / 10;  // Calculate the row index in the subsection grid
    int col = pos.x / 10;  // Calculate the column index in the subsection grid

    return row * 5 + col;  // 5 subsections per row
}

std::pair<int, int> Belief::indexToMainCoordinate(int index) {
    std::pair<int, int> val =  indexToCoordinate(index);
    val.first+=5;
    val.second+=5;
    return val;    
}

std::pair<int, int> Belief::indexToCoordinate(int index) {
    int row = index / 5;    // 5 subsections per row
    int col = index % 5;    // Column index is the remainder

    // Convert the row and column back to the coordinates of the top-left cell of the subsection
    return std::make_pair(col * 10, row * 10);
}


void Belief::updateBelief(Coordinate pos, float prob){
    int index = coordinateToIndex(pos);
    belief[index] += prob;
}

void Belief::resetBelief(){
    for(auto& element : belief){
        element = 0.0f;
    }
}

/**
*   1. Find the index of the maximum value in the belief vector 
*   2. Convert the index to the top-left coordinate of the subsection
*   3. Calculate the center of the subsection; Since each subsection is 10x10, the center will be 5 units away from the top-left coordinate on both axes
*/
std::pair<int, int> Belief::getMostFavorablePosition() {
    int maxIndex = std::distance(belief.begin(), std::max_element(belief.begin(), belief.end()));
    std::pair<int, int> topLeftCoordinate = indexToMainCoordinate(maxIndex);
    return std::make_pair(topLeftCoordinate.first + 5, topLeftCoordinate.second + 5);
}

std::vector<std::pair<int, int>> Belief::getFavoritePositions(const std::pair<int, int>& position){
    int currentPosition = coordinateToIndex({position.first, position.second});
    float maxBelief = *std::max_element(belief.begin(), belief.end());
    std::vector<std::pair<int, int>> candidates;
    std::vector<std::pair<int, int>> closestCandidates;
    double minDistance = std::numeric_limits<double>::max();

        // First, find the maximum belief value
    for (int i = 0; i < ELEMENTS; ++i) {
        if (belief[i] == maxBelief) {
            if(i == currentPosition){
                candidates.clear();
                candidates.push_back(position);
                return candidates;      
            }

            candidates.push_back(indexToMainCoordinate(i));
        }
    }

    std::pair<int, int> currentCenter = indexToMainCoordinate(currentPosition);
    for (const auto& candidate : candidates) {
        double distance = Utility::heuristic(currentCenter, candidate);

        if (distance < minDistance) {
            minDistance = distance;
            closestCandidates.clear();
        }

        if (distance == minDistance) {
            closestCandidates.push_back(candidate);
        }
    }

    return closestCandidates;
}