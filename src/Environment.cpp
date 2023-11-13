#include "../headers/Environment.h"
#include "../headers/Utility.h"
#include "../headers/DeterministicBot.h"
#include "../headers/ProbabilisticBot.h"

#include <stdexcept>
#include <iostream>


Environment::Environment(int shipSize, int range_mod, int alpha)
    : ship(shipSize), range_mod(range_mod), alpha(alpha) {
}

std::string Environment::getDistanceFor(const std::string& id){
    for(const auto& bot: bots){
        if(bot->getID() == id){
            return std::to_string(bot->getTotalDistance());
        }
    }

    return "Not Found";
}

bool Environment::isActive(){
    return ship.hasLeaks();
}

std::vector<std::vector<int>> Environment::getGridFor(const std::string& id) {
    // Make a copy of the ship's grid
    std::vector<std::vector<int>> newGrid = ship.getGrid();

    // Find the bot with the matching id
    for (const auto& bot : bots) {
        if (bot->getID() == id) {
            // Get the goal positions and mark them on the new grid   

            auto botPath = bot->getPathTaken();
            for (const auto& position : botPath){
                if (position.first >= 1 && position.first < newGrid.size()-1 &&
                    position.second >= 1 && position.second < newGrid[position.first].size()-1){
                    if(newGrid[position.first][position.second] == 0){
                        newGrid[position.first][position.second] = 4;
                    }

                }

            }

            auto pointsWhenIScanned = bot->getPointsScanned();
            for(const auto& position : pointsWhenIScanned){
                newGrid[position.first][position.second] = 5;
            }


            auto goalPositions = bot->getGoalPositions();
            for (const auto& position : goalPositions) {
                if (position.first >= 0 && position.first < newGrid.size() &&
                    position.second >= 0 && position.second < newGrid[position.first].size()) {
                    newGrid[position.first][position.second] = 2;
                }
            }

            // Mark the bot's position on the new grid
            auto botPosition = bot->getInitialPosition();
            if (botPosition.first >= 0 && botPosition.first < newGrid.size() &&
                botPosition.second >= 0 && botPosition.second < newGrid[botPosition.first].size()) {
                newGrid[botPosition.first][botPosition.second] = 3;
            }





            // Since we found the bot, no need to continue looping
            break;
        }
    }

    return newGrid;
}


void Environment::addBot(std::string type, std::string id, bool mode) {
    std::pair<int, int> pos;
    do{
        pos = Utility::generateCoordinate();
    }while(!ship.positionIsOpen(pos.first, pos.second));

    std::unique_ptr<Bot> bot;
    if (type == "deterministic") {
        bot = std::make_unique<DeterministicBot>(pos, range_mod, alpha, id, mode);
    } else if (type == "probabilistic"){
        bot = std::make_unique<ProbabilisticBot>(pos, range_mod, alpha, id, mode);
    }else{
        throw std::runtime_error("Unknown bot type: " + type); // Throws a runtime_error with the unknown type
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
        }else if(bot->getType() == "Probabilistic"){

            ship.probabilisticStart(bot->getOpenPositions(), bot->getID());
            std::vector<std::pair<int, int>> idk;
            ship.addLeak(idk);
            if(!bot->isDumb()){
                ship.addLeak(idk);
            }
        }


//        int count = 0;&& count < 2
        while(bot->isActive() ){
            bool leakDetected = bot->performScan(ship);
            if(leakDetected){
                bot->performDetected(ship);
            }else{
                bot->performNotDetected(ship);
            }
            try{
                bot->moveToNextLocation(ship);
            }catch (const std::exception& e) {
                std::cout << "Error: bot literally can't move" << e.what() << std::endl;
                break;
            }


//            count++;
        }
        std::vector<std::pair<int, int>> leakPositions = ship.reset();
        bot->setLeakPositions(leakPositions);
    }
}

void Environment::runTestSimulation(){
    // Iterate over the list of bots to find the one with ID "bot1"
    for (auto& bot : bots) {
        if(ship.firstRoundTest()){
//            DeterministicBot* detBot = dynamic_cast<DeterministicBot*>(bot.get());
//            ship.addLeak(detBot->getSpawnRadius());
            ship.probabilisticStart(bot->getOpenPositions(), bot->getID());
            std::vector<std::pair<int, int>> idk;
            ship.addLeak(idk);
            if(!bot->isDumb()){
                ship.addLeak(idk);
            }
            bot->setLeakPositions( ship.getPositionOfLeaks() );
        }


        bool signal = bot->performScan(ship);
        std::cout << "signal: ";
        if(signal){
            std::cout << "True" << std::endl;
            bot->performDetected(ship);
        }else{
            std::cout << "False" << std::endl;
            bot->performNotDetected(ship);
        }
        try{
            bot->moveToNextLocation(ship);
        }catch (const std::exception& e) {
            std::cout << "Error: bot literally can't move" << e.what() << std::endl;
            break;
        }

        break;
    }
}