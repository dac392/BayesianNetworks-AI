#ifndef DETERMINISTIC_BOT_H
#define DETERMINISTIC_BOT_H

#include "Bot.h"
#include <algorithm>

class DeterministicBot : public Bot {

public:
    // using Bot::Bot;
    DeterministicBot::DeterministicBot(const std::pair<int, int>& startPos, int range_mod, int alpha, const std::string& id, bool dumb);

    std::string getType() override;
    bool scan(std::vector<std::pair<int, int>> leaks) override;
    std::vector<std::pair<int, int>> getPosibleLeakPositions() override;
    std::vector<std::pair<int, int>> updatePosiblePositions() override;
    void moveToNextLocation() override;

    std::vector<std::pair<int, int>> getSpawnRadius();

};

#endif // DETERMINISTIC_BOT_H
