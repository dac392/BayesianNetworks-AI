#include "DeterministicBot.h"

DeterministicBot::DeterministicBot(const std::pair<int, int>& startPos, int range_mod, int alpha, std::string id)
: currentPosition(startPos), sensor(range_mod, alpha), id(id){
}

void DeterministicBot::move(int x, int y) {
    // Implementation for deterministic move
    currentPosition = {x, y};
}

bool DeterministicBot::scan(int x, int y) {
    // Implementation for deterministic scan
    return false;
}

void DeterministicBot::performAction() {
    // Implementation for deterministic action
    // E.g., move to a certain location deterministically
}

std::pair<int, int> DeterministicBot::getCurrentPosition() const {
    return currentPosition;
}

Sensor& DeterministicBot::getSensor() {
    return sensor;
}
