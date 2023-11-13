#include "../headers/Utility.h"
#include <cstdlib>
#include <ctime>
#include <algorithm> // For std::shuffle
#include <iterator>  // For std::begin, std::end
#include <random>    // For std::default_random_engine

std::pair<int, int> Utility::generateCoordinate(){
    // Seed the random number generator.
    // Note: You normally only seed once, so this would go elsewhere in a full program.
    static bool seeded = false;
    if (!seeded) {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        seeded = true;
    }

    // Generate two random numbers between 1 and 48
    int x = std::rand() % 48 + 1;
    int y = std::rand() % 48 + 1;

    // Create a vector with the generated coordinates and return it
    std::pair<int,int> coordinate = {x, y};
    return coordinate;
}

std::string Utility::hashFunction(const std::pair<int, int>& p) {
    return "(" + std::to_string(p.first) + ", " + std::to_string(p.second) + ")";
}

int Utility::heuristic(const std::pair<int, int>& a, const std::pair<int, int>& b) {
    return std::abs(a.first - b.first) + std::abs(a.second - b.second);
}

std::pair<int, int> Utility::shufflePositions(std::vector<std::pair<int, int>>& positions) {
    if(positions.empty()){
        throw std::runtime_error("Utility::shuffle positions is empty"); // or any other error handling
    }

    // Initialize a random number generator
    std::random_device rd;
    std::mt19937 g(rd());

    // Shuffle the positions vector
    std::shuffle(positions.begin(), positions.end(), g);

    // Assuming you want to return the first element after shuffling

    return positions.front();

}


void Utility::removePosition(std::vector<std::pair<int, int>>& vec, const std::pair<int, int>& elem) {
    auto it = std::find(vec.begin(), vec.end(), elem);
    if (it != vec.end()) {
        vec.erase(it);
    }
}