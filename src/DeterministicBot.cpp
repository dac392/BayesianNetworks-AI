#include "../headers/DeterministicBot.h"

std::string getType(){
    return "Deterministic";
}

bool scan(int x, int y) {
    return false;
}
void performAction()  {

}

std::vector<std::pair<int, int>> DeterministicBot::getSpawnRadius() {
    std::vector<std::pair<int, int>> spawnRadius;
    
    // Get the range of the sensor which defines the radius 'k'
    int k = this->getSensor().getRange();

    // Determine the bounds around the bot's current position
    int startX = currentPosition.first - k;
    int endX = currentPosition.first + k;
    int startY = currentPosition.second - k;
    int endY = currentPosition.second + k;

    // Iterate over the square area
    for (int x = startX; x <= endX; ++x) {
        for (int y = startY; y <= endY; ++y) {
            // Calculate the Manhattan distance from the current position to the (x, y) coordinate
            int distance = std::abs(x - currentPosition.first) + std::abs(y - currentPosition.second);
            // If the distance is less than or equal to k, it's within the spawn radius
            if (distance <= k) {
                spawnRadius.emplace_back(x, y);
            }
        }
    }

    return spawnRadius;
}
