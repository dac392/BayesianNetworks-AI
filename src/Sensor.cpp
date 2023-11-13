#include "../headers/Sensor.h"
#include <random>
#include <iostream>

// Constructor implementation
Sensor::Sensor(int range_mod, int alpha)
    : k(range_mod),alpha(alpha),
    generator(std::random_device{}()), distribution(0.0f, 1.0f) {
    range = (2*k+1);
}

// detect method implementation
bool Sensor::detect(int distance) {
    float probabilityOfDetection = std::exp(-alpha * (distance - 1));
    float randomValue = distribution(generator);
    return randomValue <= probabilityOfDetection;
}

float Sensor::getProbability(int distance){
    return std::exp(-alpha * (distance - 1));
}

// Getter for range
int Sensor::getRange() const {
    return range;
}

// Setter for range
void Sensor::setRange(int newRange) {
    range = newRange;
}

// Getter for probabilityModel
float Sensor::getAlpha() const {
    return alpha;
}