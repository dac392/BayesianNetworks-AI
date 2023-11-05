#include "Sensor.h"
#include <random>

// Constructor implementation
Sensor::Sensor(int range, int alpha) 
    : range(range), 
      generator(std::random_device{}()), distribution(0.0f, 1.0f) {}

// detect method implementation
bool Sensor::detect(int distance) {
    if (distance > range) {
        // If the leak is outside the range of the sensor, it cannot be detected
        return false;
    }

    // Calculate the probability of detection based on the distance
    float probabilityOfDetection = probabilityModel * (range - distance) / range;

    // Generate a random number and compare it to the detection probability
    float randomValue = distribution(generator);
    return randomValue < probabilityOfDetection;
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
float Sensor::getProbabilityModel() const {
    return probabilityModel;
}

// Setter for probabilityModel
void Sensor::setProbabilityModel(float newProbabilityModel) {
    probabilityModel = newProbabilityModel;
}
