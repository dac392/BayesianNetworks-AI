#ifndef PROBABILISTIC_BOT_H
#define PROBABILISTIC_BOT_H

#include "Bot.h"

class ProbabilisticBot : public Bot {
public:
    ProbabilisticBot(const std::pair<int, int>& startPos, int range_mod, int alpha, const std::string& id, bool dumb);
    std::string getType() override;

    virtual bool performScan(Ship& ship) override;
    virtual void performDetected(Ship& ship) override;
    virtual void performNotDetected(Ship& ship) override;
    void moveToNextLocation(Ship& ship) override;

    bool scan(Ship& ship);
    void correctedUpdate(Ship& ship, bool signalDetected);
    void updateProbabilities(Ship& ship, bool signalDetected);
    void naiveNextStep(Ship& ship);
    void intelligentStep(Ship& ship);

};

#endif // PROBABILISTIC_BOT_H
