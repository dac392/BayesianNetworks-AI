#ifndef BOT_H
#define BOT_H

#include "Sensor.h"
#include <utility>

class Bot {

public:
    virtual ~Bot() = default;

    // Virtual methods to be implemented by derived classes.
    virtual void move(int x, int y) = 0;
    virtual bool scan(int x, int y) = 0;
    virtual void performAction() = 0;

    // Accessor methods for the attributes all bots have.
    virtual std::pair<int, int> getCurrentPosition() const = 0;
    virtual Sensor& getSensor() = 0;
};

#endif // BOT_H
