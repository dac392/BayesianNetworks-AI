#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "Ship.h"
#include "Bot.h"
#include <vector>
#include <memory> // For std::unique_ptr

class Environment {
private:
    Ship ship;
    std::vector<std::unique_ptr<Bot>> bots;

public:
    Environment(int shipSize);
    ~Environment();

    void addBot(std::unique_ptr<Bot> bot);
    void runSimulation();

    // Other environment-related methods
};

#endif // ENVIRONMENT_H
