#ifndef PROBABILISTIC_BOT_H
#define PROBABILISTIC_BOT_H

#include "Bot.h"

class ProbabilisticBot : public Bot {
private:
    std::string id;
    std::pair<int, int> currentPosition;
    Sensor sensor;

public:
    ProbabilisticBot(const std::pair<int, int>& startPos, int range_mod, int alpha, std::string id);

    // Override Bot methods
    void move(int x, int y) override;
    bool scan(int x, int y) override;
    void performAction() override;

    // Override accessor methods
    std::pair<int, int> getCurrentPosition() const override;
    Sensor& getSensor() override;
};

#endif // PROBABILISTIC_BOT_H
