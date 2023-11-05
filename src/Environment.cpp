#include "../headers/Environment.h"

Environment::Environment(int shipSize, int range_mod, int alpha) 
            : ship(shipSize), range_mod(range_mod), alpha(alpha) {
    // Initialize ship and other environment-specific setup
}

void Environment::addBot(std::string type, std::string id, bool mode) {
    std::pair<int, int> pos = Utility::generateCoordinate();
    std::unique_ptr<Bot> bot;
    if (type == "deterministic") {
        bot = std::make_unique<DeterministicBot>(pos, range_mod, alpha, id, mode);
    } else {
        bot = std::make_unique<ProbabilisticBot>(pos, range_mod, alpha, id, mode);
    }

    bot->populateOpenPositions(ship.getGrid());
    bots.push_back(std::move(bot));
}


void Environment::runSimulation() {
    // Logic to run the simulation with bots and the ship
    // This will typically involve iterating over the bots and invoking their behavior
    for (auto& bot : bots) {
        
        if(bot->getType() == "Deterministic"){
            DeterministicBot* detBot = dynamic_cast<DeterministicBot*>(bot.get());
            ship.addLeak(detBot->getSpawnRadius());
            if(!bot->isDumb()){
                ship.addLeak(detBot->getSpawnRadius());
            }
        }
        // probably do something if its Probabilistic

        std::vector<std::pair<int, int>> posibleLeakPositions;


        // concider moving most of this into a bot specific method performAction()
        while(bot->isActive()){
            bool leakDetected = bot->scan(ship.getPositionOfLeaks());
            if(leakDetected){
                posibleLeakPositions = bot->getPosibleLeakPositions();
            }else{
                posibleLeakPositions = bot->updatePosiblePositions();
            }

            bot->moveToNextLocation();
        }
        std::vector<std::pair<int, int>> leakPositions = ship.reset();
        bot->setLeakPositions(leakPositions);
    }

    
}

// Implementation of other environment-related methods
