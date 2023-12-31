#ifndef DETERMINISTIC_BOT_H
#define DETERMINISTIC_BOT_H

#include "Bot.h"
#include "Spiral.h"
#include <algorithm>

/**
 * handles Deterministic approach
*/
class DeterministicBot : public Bot {

private:
    std::vector<std::pair<int, int>> not_yet_explored;
    Spiral heuristic;
public:
    //using Bot::Bot;
    DeterministicBot(const std::pair<int, int>& startPos, int range_mod, float alpha, const std::string& id, bool dumb);
    std::string getType() override;

    virtual bool performScan(Ship& ship) override;
    virtual void performDetected(Ship& ship) override;
    virtual void performNotDetected(Ship& ship) override;
    void moveToNextLocation(Ship& ship) override;

    void naiveApproach(Ship& ship);
    void spiralApproach(Ship& ship);
    std::vector<std::pair<int, int>> getSpawnRadius();
    bool scan(std::vector<std::pair<int, int>> leaks);
    void recordDifference();
    void removePositionsOutOfRange();
    void removePositionsInRange();

};

#endif // DETERMINISTIC_BOT_H
