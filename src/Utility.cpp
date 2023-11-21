#include "../headers/Utility.h"
#include <cstdlib>
#include <ctime>
#include <algorithm> // For std::shuffle
#include <iterator>  // For std::begin, std::end
#include <random>    // For std::default_random_engine
#include <queue>
#include <chrono>
#include <sstream>

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

    return positions.front();

}

void Utility::error(const std::string& message){
    throw std::runtime_error(message+"\n");
}

int Utility::cast(int size){
    return static_cast<std::vector<std::vector<int> >::size_type>(size);
}


void Utility::removePosition(std::vector<std::pair<int, int>>& vec, const std::pair<int, int>& elem) {
    auto it = std::find(vec.begin(), vec.end(), elem);
    if (it != vec.end()) {
        vec.erase(it);
    }
}

std::string Utility::generateTimestampID() {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();

    std::ostringstream oss;
    oss << duration.count(); // Append other unique data as needed
    return oss.str();
}

bool Utility::isValid(int x, int y, int dim){
    if(x>=0 && x<dim && y>=0 && y<dim){
        return true;
    }

    return false;
}

std::pair<int, int> Utility::getClosestPosition(const Coordinate& start, std::vector<std::pair<int, int>>& open){
    const std::vector<std::pair<int, int>> DIRECTIONS = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    std::queue<Coordinate> q;
    q.push(start);

    while(!q.empty()){
        Coordinate current = q.front();
        q.pop();
        for(const auto& dir : DIRECTIONS){
            Coordinate next = {current.x + dir.first, current.y + dir.second};
            if(std::find(open.begin(), open.end(), std::make_pair(next.x, next.y))!=open.end()){
                return std::make_pair(next.x, next.y);
            }
            q.push(next);

        }
    }

    return Utility::shufflePositions(open);
}