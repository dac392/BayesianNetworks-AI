#include "../headers/DeterministicBot.h"
DeterministicBot::DeterministicBot(const std::pair<int, int>& startPos, int range_mod, int alpha, const std::string& id, bool dumb)
    : Bot(startPos, range_mod, alpha, id, dumb) {
    // ... additional initialization ...
}

bool DeterministicBot::performScan(Ship& ship){
    return scan(ship.getPositionOfLeaks());
}
void DeterministicBot::performDetected(Ship& ship){
    removePositionsOutOfRange();
}
void DeterministicBot::performNotDetected(Ship& ship){

    ship.markAsSeen(currentPosition, sensor.getRange());
    removePositionsInRange();

}


std::string DeterministicBot::getType(){
    return "Deterministic";
}

bool DeterministicBot::scan(std::vector<std::pair<int, int>> leaks) {
    totalActions++;
    bool leakDetected = false;

    // Get the sensor's range for checking leaks
    int sensorRange = this->getSensor().getRange();

    pointsWhereIScanned.emplace_back(currentPosition);

    // Calculate the bounds of the sensor range
    int startX = currentPosition.first - sensorRange;
    int endX = currentPosition.first + sensorRange;
    int startY = currentPosition.second - sensorRange;
    int endY = currentPosition.second + sensorRange;

    // Add all positions within sensor range to the path
    for (int x = startX; x <= endX; ++x) {
        for (int y = startY; y <= endY; ++y) {
            int distance = std::abs(x - currentPosition.first) + std::abs(y - currentPosition.second);
            if (distance <= sensorRange) {
                path.emplace_back(x, y);  // Add position to path
            }
        }
    }

    // Check for leaks within the sensor range
    for (const auto& leak : leaks) {
        int leakX = leak.first;
        int leakY = leak.second;
        int distance = std::abs(leakX - currentPosition.first) + std::abs(leakY - currentPosition.second);

        if (distance <= sensorRange) {
            leakDetected = true;
            break;  // No need to continue checking once a leak is found
        }
    }

    return leakDetected;
}


void  DeterministicBot::removePositionsOutOfRange(){
    std::vector<std::pair<int, int>> possibleLeakPositions;
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

}


void DeterministicBot::removePositionsInRange() {

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

}

void DeterministicBot::moveToNextLocation(Ship& ship) {
    std::vector<std::pair<int, int>> candidates = ship.getClosestReachable(currentPosition, openPositions);

    // Choose a random position from the candidates
    if (!candidates.empty()) {
        // Random device and generator
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(candidates.begin(), candidates.end(), g); // Shuffle the candidates

        std::pair<int, int> nextLocation = candidates.front(); // Take the first after shuffling


        totalActions += ship.getDistanceFrom(currentPosition, nextLocation); // Add the minimum distance to total actions
        ship.markAsSeen(nextLocation, -1);
        currentPosition = nextLocation; // Move to the chosen position
        if(ship.canPlugLeak(currentPosition)){
            bool done = ship.plugLeak(currentPosition);
            if(done){
                active = false;
            }else{
                openPositions = ship.bot5Correction();
            }
        }


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
