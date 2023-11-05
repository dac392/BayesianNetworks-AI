#ifndef DETERMINISTIC_BOT_H
#define DETERMINISTIC_BOT_H

#include "Bot.h"

class DeterministicBot : public Bot {
private:
    std::pair<int, int> currentPosition;
    Sensor sensor;

public:
    DeterministicBot(const std::pair<int, int>& startPos, const Sensor& sensor);

    // Override Bot methods
    void move(int x, int y) override;
    bool scan(int x, int y) override;
    void performAction() override;

    // Override accessor methods
    std::pair<int, int> getCurrentPosition() const override;
    Sensor& getSensor() override;
};

#endif // DETERMINISTIC_BOT_H