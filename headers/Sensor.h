#ifndef SENSOR_H
#define SENSOR_H

#include <random>

class Sensor {
private:
    int range;
    int k;
//    float probabilityModel; // The probability model coefficient for detection
    float alpha;
    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution;

public:
    Sensor(int range, int alpha);
    bool detect(int distance);
    float getProbability(int distance);

    // Getters and setters
    int getRange() const;
    void setRange(int newRange);
    float getAlpha() const;

};

#endif // SENSOR_H