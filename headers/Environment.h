#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "Ship.h"
#include "Bot.h"
#include <vector>
#include <memory> // For std::unique_ptr
#include <string>

class Environment {
private:
    Ship ship;
    std::vector<std::unique_ptr<Bot>> bots;
    int range_mod;
    int alpha;

public:
    Environment(int shipSize, int range_mod, int alpha);

    void addBot(std::string type, std::string id, bool mode);
    void start();
    void individualSimulation();
    void runSimulation();
    void runTestSimulation();
    std::vector<std::vector<int>> getGridFor(const std::string& id);
    std::string getDistanceFor(const std::string& id);
    bool isActive();

    // Other environment-related methods
};

#endif // ENVIRONMENT_H
