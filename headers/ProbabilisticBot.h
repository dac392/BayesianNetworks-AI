#ifndef PROBABILISTIC_BOT_H
#define PROBABILISTIC_BOT_H

#include "Bot.h"

class ProbabilisticBot : public Bot {
public:
    using Bot::Bot; // Use the constructor from Bot

    // Implement any specific methods that differ from Bot's implementation
    std::string getType() override;
    bool scan(std::vector<std::pair<int, int>> leaks) override ;
    void performAction() override;

    // ... other methods specific to ProbabilisticBot ...
};

#endif // PROBABILISTIC_BOT_H
