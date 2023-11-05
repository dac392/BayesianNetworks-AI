#ifndef DETERMINISTIC_BOT_H
#define DETERMINISTIC_BOT_H

#include "Bot.h"

class DeterministicBot : public Bot {

public:
    using Bot::Bot;

    std::string getType() override;
    bool scan(int x, int y) override;
    void performAction() override;
    std::vector<std::pair<int, int>> DeterministicBot::getSpawnRadius();

};

#endif // DETERMINISTIC_BOT_H
