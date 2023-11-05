#include "../headers/Leak.h"

Leak::Leak(int x, int y) : x(x), y(y), state(true) {
    // The constructor initializes the leak with the provided location and sets the state to true (active)
}

Leak::~Leak() {
    // The destructor currently does not need to do anything special
}

int Leak::getX() const {
    return x;
}

int Leak::getY() const {
    return y;
}

bool Leak::isActive() const {
    return state;
}

void Leak::plug() {
    state = false; // The leak is plugged, so we set the state to false
}
