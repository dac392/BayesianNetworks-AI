#include "../headers/DeterministicBot.h"

std::string getType(){
    return "Deterministic";
}

bool DeterministicBot::scan(std::vector<std::pair<int, int>> leaks) {
    // Get the sensor's range for checking leaks
    int sensorRange = this->getSensor().getRange();
    
    // Iterate over each leak in the list
    for (const auto& leak : leaks) {
        // Calculate the Manhattan distance from the bot's current position to the leak's position
        int leakX = leak.first;
        int leakY = leak.second;
        int distance = std::abs(leakX - currentPosition.first) + std::abs(leakY - currentPosition.second);
        
        // Check if the leak is within the sensor range
        if (distance <= sensorRange) {
            // If the leak is detected, return true
            return true;
        }
    }
    
    // If no leaks were detected within range, return false
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
