#include "../headers/Leak.h"

Leak::Leak(std::pair<int, int> position) : position(position), state(true) {
    // The constructor initializes the leak with the provided location and sets the state to true (active)
}

std::pair<int,int> Leak::getPosition() const{
    return position;
}

bool Leak::isActive() const {
    return state;
}

void Leak::plug() {
    state = false; // The leak is plugged, so we set the state to false
}

void Leak::reset(){
    state = true;
}
