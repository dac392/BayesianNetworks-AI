#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "Ship.h"
#include "Bot.h"
#include "Utility.h"
#include "DeterministicBot.h"
#include "ProbabilisticBot.h"
#include <vector>
#include <memory> // For std::unique_ptr

class Environment {
private:
    Ship ship;
    std::vector<std::unique_ptr<Bot>> bots;
    int range_mod;
    int alpha;

public:
    Environment(int shipSize, int range_mod, int alpha);

    void addBot(std::string type, std::string id, bool mode);
    void runSimulation();
    std::vector<std::pair<int, int>> getSpawnRadius();

    // Other environment-related methods
};

#endif // ENVIRONMENT_H
