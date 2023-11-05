#include "../headers/ProbabilisticBot.h"

std::string ProbabilisticBot::getType(){
    return "Probabilistic";
}

bool ProbabilisticBot::scan(std::vector<std::pair<int, int>> leaks){
    totalActions++;
    // Iterate over each leak in the list
    for (const auto& leak : leaks) {
        // Calculate the Manhattan distance from the bot's current position to the leak's position
        int distance = std::abs(leak.first - currentPosition.first) + std::abs(leak.second - currentPosition.second);
        
        // Attempt to detect the leak based on the sensor's probability model
        if (sensor.detect(distance)) {
            // If the leak is detected, return true
            return true;
        }
    }
    
    // If no leaks were detected after checking all, return false
    return false;
}

std::vector<std::pair<int, int>> ProbabilisticBot::getPosibleLeakPositions(){

}


void ProbabilisticBot::moveToNextLocation(){

}