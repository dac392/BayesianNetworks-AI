#ifndef SENSOR_H
#define SENSOR_H

#include <random>

class Sensor {
private:
    int range; // The maximum range of the sensor
    float probabilityModel; // The probability model coefficient for detection
    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution;

public:
    Sensor(int range, int alpha);
    bool detect(int distance);

    // Getters and setters
    int getRange() const;
    void setRange(int newRange);
    float getProbabilityModel() const;
    void setProbabilityModel(float newProbabilityModel);
};

#endif // SENSOR_H
