#include "Environment.h"

Environment::Environment(int shipSize, int range_mod, int alpha) 
            : ship(shipSize), range_mod(range_mod), alpha(alpha) {
    // Initialize ship and other environment-specific setup
}

void Environment::addBot(std::string type, std::string id) {
    std::vector<int> pos = Utility::generateCoordinate();
    std::unique_ptr<Bot> bot;
    if (type == "deterministic") {
        bot = std::make_unique<DeterministicBot>(pos, range_mod, alpha, id);
    } else {
        bot = std::make_unique<ProbabilisticBot>(pos, range_mod, alpha, id);
    }

    bots.push_back(std::move(bot));
}

void Environment::runSimulation() {
    // Logic to run the simulation with bots and the ship
    // This will typically involve iterating over the bots and invoking their behavior
    for (auto& bot : bots) {
        // The bot could move, scan, and perform actions
        bot->performAction();
        // You could implement logic here that checks the state of the ship, 
        // determines if the bot's actions have an effect, etc.
    }
    // Perform other simulation steps, such as advancing time, handling the ship state, etc.
}

// Implementation of other environment-related methods
