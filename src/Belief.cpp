#include "../headers/Belief.h"
//#include "Utility.h"
#include <limits>

#define ELEMENTS 25
#define SUBSECTION_SIZE = 10
#define GRID_SIZE = 50

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

std::pair<int, int> Belief::getFavoritePositions(const std::pair<int, int>& position){
    int indexOfPos = coordinateToIndex({position.first, position.second});
    std::vector<int> neighbors = getNeighborSubsections(position);
    neighbors.push_back(indexOfPos);
    float maxProb = 0.0f;
    int maxIndex = -1;
    for(const auto& neighbor : neighbors){
        if(belief[neighbor]> maxProb){
            maxProb = belief[neighbor];
            maxIndex = neighbor;
        }
    }
    std::pair<int, int> best = (maxIndex == indexOfPos)? position : indexToCoordinate(maxIndex);
    return best;
}


std::vector<int> Belief::getNeighborSubsections(const std::pair<int, int>& position) {
    int index = coordinateToIndex({position.first, position.second});
    std::vector<int> neighbors;

    // Determine row and column of the current subsection
    int row = index / 5;
    int col = index % 5;

    // Add neighboring subsections, checking for grid boundaries
    for (int r = row - 1; r <= row + 1; ++r) {
        for (int c = col - 1; c <= col + 1; ++c) {
            // Check if the neighbor is within grid bounds and not the current subsection itself
            if (r >= 0 && r < 5 && c >= 0 && c < 5 && !(r == row && c == col)) {
                neighbors.push_back(r * 5 + c);
            }
        }
    }

    return neighbors;
}