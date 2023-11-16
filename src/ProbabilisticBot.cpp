#include "../headers/ProbabilisticBot.h"
#include "../headers/Utility.h"
#include <stdexcept>

// hardcoding dimensions of 50x50 on bayesian network;
ProbabilisticBot::ProbabilisticBot(const std::pair<int, int>& startPos, int range_mod, float alpha, const std::string& id, bool dumb)
    : Bot(startPos, range_mod, alpha, id, dumb), bayesian_network(50) {
}

void ProbabilisticBot::populateOpenPositions(const std::vector<std::vector<int>>& grid){
    Bot::populateOpenPositions(grid);
    bayesian_network.init(openPositions, (id=="bot9" || id=="bot8"));
}

std::string ProbabilisticBot::getType(){
    return "Probabilistic";
}

bool ProbabilisticBot::performScan(Ship& ship){
    totalActions++;
    return scan(ship);
}

void ProbabilisticBot::performDetected(Ship& ship){
    if(id == "bot9" || id == "bot8"){
        correctedUpdate(ship, true);
    }
    updateProbabilities(ship, true);    //always true if we're here



}

void ProbabilisticBot::performNotDetected(Ship& ship){
    if(id == "bot9" || id == "bot8"){
        correctedUpdate(ship, false);
    }

    updateProbabilities(ship, false);    //always false if we're here

}

bool ProbabilisticBot::scan(Ship& ship){
    std::vector<std::pair<int, int>> leaks = ship.getPositionOfLeaks();
    bool signal = false;
    for(auto& leak: ship.getLeaks()){
        if(leak.isActive()){
            int distance = ship.getDistanceFrom(currentPosition, leak.getPosition());
            signal = signal || sensor.detect(distance);
        }
    }

    return signal;
}

void ProbabilisticBot::correctedUpdate(Ship& ship, bool signalDetected){
    if(ship.almostDone()){
        bayesian_network.lateGameUpdate(ship.getDistances(), sensor, currentPosition, signalDetected);
    }else{
        bayesian_network.updatePairProbability(ship.getDistances(), sensor, currentPosition, signalDetected);
    }

}

void ProbabilisticBot::updateProbabilities(Ship& ship, bool signalDetected){

    float sumProbabilities = 0.0;

    for(auto& pos : openPositions){
        int distance = ship.getDistanceFrom(currentPosition, pos);
        float probabilityOfBeep = sensor.getProbability(distance);
        if(signalDetected){
            bayesian_network.scaleProbabilities(pos, probabilityOfBeep);
        }else{
            bayesian_network.scaleProbabilities(pos, 1-probabilityOfBeep);
        }

        sumProbabilities+=bayesian_network.getProbabilityAt(pos);
    }


    bayesian_network.normalizeProbabilities(sumProbabilities);


}




void ProbabilisticBot::moveToNextLocation(Ship& ship){
    if(id == "bot9" || id=="bot8"){
        intelligentStep(ship);
    }else{
        naiveNextStep(ship);
    }

}

void ProbabilisticBot::intelligentStep(Ship& ship){ //contains(open, position)
    std::vector<std::pair<int, int>> candidates;
    std::vector<std::pair<int, int>> allMostProbable;
    if(ship.almostDone()){
        allMostProbable = bayesian_network.getIntelligentStep(currentPosition);  //actually, just gets the highest probabilityPair list
    }else{
        allMostProbable = bayesian_network.getHighestProbabilityList();

    }
    candidates = ship.getMostProbable(currentPosition, allMostProbable);


    std::pair<int, int> nextPosition = Utility::shufflePositions(candidates);
    std::vector<std::pair<int, int>> pathToNextPosition = ship.getShortestPath(currentPosition, nextPosition);
    bool foundLeak = false;
    for(const auto& pos : pathToNextPosition){

        currentPosition = pos;
        if(ship.canPlugLeak(currentPosition)){
            bool done = ship.plugLeak(currentPosition);

            if(done){
                active = false;
                return;
            }else{  // we change to using bayesian instead of normal matrix

                foundLeak = true;
            }


        }
        if(foundLeak){
            // remove double probability not containing current
            bayesian_network.narrowDownSearchSpace(currentPosition);
        }else if(ship.almostDone()){
            // can only remove one posibility at a time once we found the first leaks
            bayesian_network.smallerRemove(currentPosition);
        }else{
            // remove double probability
            bayesian_network.remove(currentPosition);
        }

        //ship.normalizePairs(openPositions);// no longer needed, I am doing this inside of remove now // this can difinitely be optimized; also might have to change how you normalize; check when not drunk

        // remore single probability
        Utility::removePosition(openPositions, currentPosition);
        path.emplace_back(pos);

        // update normal probabilities
        float prob = bayesian_network.getProbabilityAt(currentPosition);
        float norm = prob/openPositions.size();     // this doesn't feel right; double check real quick
        bayesian_network.updateProbabilities(currentPosition, norm);

        totalActions+=1;

        if(foundLeak){
            return;
        }
    }
}

void ProbabilisticBot::naiveNextStep(Ship& ship){
    std::vector<std::pair<int, int>> candidates = ship.getMostProbable(currentPosition, openPositions);

    std::pair<int, int> nextPosition = Utility::shufflePositions(candidates);
    std::vector<std::pair<int, int>> pathToNextPosition = ship.getShortestPath(currentPosition, nextPosition);
    // plan path from current to next position
    for(const auto& pos : pathToNextPosition){
        currentPosition = pos;
        if(ship.canPlugLeak(currentPosition)){
            if(ship.plugLeak(currentPosition)){ // true only if there are no more leaks active
                active = false;
                return;
            }
        }
        Utility::removePosition(openPositions, currentPosition);
        path.emplace_back(pos);
        float prob = bayesian_network.getProbabilityAt(currentPosition);
        float norm = prob/openPositions.size();
        bayesian_network.updateProbabilities(currentPosition, norm);
        totalActions+=1;
    }

}
