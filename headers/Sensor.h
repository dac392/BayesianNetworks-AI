#ifndef SENSOR_H
#define SENSOR_H

#include <random>

/**
 * used for both approaches in sensing
*/
class Sensor {
private:
    int range;
    int k;
    float alpha;
    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution;

public:
    Sensor(int range, float alpha);
    bool detect(int distance);
    float getProbability(int distance);

    // Getters and setters
    int getRange() const;
    void setRange(int newRange);
    float getAlpha() const;

};

#endif // SENSOR_H