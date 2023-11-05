#include "../headers/DeterministicBot.h"

std::string getType(){
    return "Deterministic";
}

bool DeterministicBot::scan(std::vector<std::pair<int, int>> leaks) {
    totalActions++;
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

std::vector<std::pair<int, int>>  DeterministicBot::getPosibleLeakPositions(){
    std::vector<std::pair<int, int>> possibleLeakPositions;

    // Get the detection range from the sensor
    int detectionRange = this->getSensor().getRange();

    // Loop through all open positions
    for (const auto& pos : openPositions) {
        // Calculate the Manhattan distance from the current position to pos
        int distance = std::abs(pos.first - currentPosition.first) + std::abs(pos.second - currentPosition.second);
        
        // If the distance is less than or equal to the detection range, add it to possibleLeakPositions
        if (distance <= detectionRange) {
            possibleLeakPositions.push_back(pos);
        }
    }

    if (!dumb) {
        // Remove all positions in possibleLeakPositions from openPositions
        openPositions.erase(
            std::remove_if(
                openPositions.begin(), 
                openPositions.end(), 
                [&possibleLeakPositions](const std::pair<int, int>& pos) {
                    return std::find(possibleLeakPositions.begin(), possibleLeakPositions.end(), pos) != possibleLeakPositions.end();
                }
            ),
            openPositions.end()
        );
    }else{
        active = false;
    }

    return possibleLeakPositions;
}


std::vector<std::pair<int, int>> DeterministicBot::updatePosiblePositions() {
    
    // Get the detection range from the sensor
    int detectionRange = this->getSensor().getRange();

    // Use the erase-remove idiom to remove positions within the detection range from openPositions
    openPositions.erase(
        std::remove_if(openPositions.begin(), openPositions.end(), 
                       [this, detectionRange](const std::pair<int, int>& pos) {
                           // Calculate the Manhattan distance
                           int distance = std::abs(pos.first - currentPosition.first) 
                                        + std::abs(pos.second - currentPosition.second);
                           // If the distance is less than or equal to detection range, we should remove it
                           return distance <= detectionRange;
                       }),
        openPositions.end()
    );

    // Since openPositions has been updated, just return it
    return openPositions;
}

void DeterministicBot::moveToNextLocation() {
    if (openPositions.empty()) {
        return;
    }

    // Initialize minimum distance with the maximum possible value
    int minDistance = std::numeric_limits<int>::max();
    std::vector<std::pair<int, int>> candidates;

    // Find the minimum distance to an open position
    for (const auto& pos : openPositions) {
        int distance = std::abs(pos.first - currentPosition.first) + std::abs(pos.second - currentPosition.second);
        if (distance < minDistance) {
            minDistance = distance;
            // Reset candidates as we found a closer distance
            candidates.clear();
            candidates.push_back(pos);
        } else if (distance == minDistance) {
            // Add position to candidates as it has the same minimum distance
            candidates.push_back(pos);
        }
    }

    // Choose a random position from the candidates
    if (!candidates.empty()) {
        // Random device and generator
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(candidates.begin(), candidates.end(), g); // Shuffle the candidates

        std::pair<int, int> nextLocation = candidates.front(); // Take the first after shuffling

        totalActions += minDistance; // Add the minimum distance to total actions
        currentPosition = nextLocation; // Move to the chosen position
    }
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
