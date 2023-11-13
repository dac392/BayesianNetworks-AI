#include "../headers/ProbabilisticBot.h"

#include "Utility.h"
#include <stdexcept>

ProbabilisticBot::ProbabilisticBot(const std::pair<int, int>& startPos, int range_mod, int alpha, const std::string& id, bool dumb)
    : Bot(startPos, range_mod, alpha, id, dumb) {
    // ... additional initialization for ProbabilisticBot ...
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
    for(auto& leak: leaks){
        int distance = ship.getDistanceFrom(currentPosition, leak);
        signal = signal || sensor.detect(distance);
    }

    return signal;
}

void ProbabilisticBot::correctedUpdate(Ship& ship, bool signalDetected){
    float normFactor = 0.0;
    for(auto& i : openPositions){
        for(auto& j : openPositions){
            if( i == j){
                continue;
            }

            int dist_ki = ship.getDistanceFrom(currentPosition, i);
            int dist_kj = ship.getDistanceFrom(currentPosition, j);

            float P_beep_ki = sensor.getProbability(dist_ki);
            float P_beep_kj = sensor.getProbability(dist_kj);
            float P_given_ij = P_beep_ki*P_beep_kj;

            float probabilityOfBeep = P_beep_ki + P_beep_kj - P_given_ij;

            if(signalDetected){
                ship.scaleBayesianPair(i, j, probabilityOfBeep);
            }else{
                ship.scaleBayesianPair(i, j , 1-probabilityOfBeep);
            }

            normFactor += ship.getPairProbabilityAt(i, j);
        }
    }

    ship.normalizeNetwork(openPositions, normFactor);

}

void ProbabilisticBot::updateProbabilities(Ship& ship, bool signalDetected){

    float sumProbabilities = 0.0;

    for(auto& pos : openPositions){
        int distance = ship.getDistanceFrom(currentPosition, pos);
        float probabilityOfBeep = sensor.getProbability(distance);
        if(signalDetected){
            ship.scaleGridPosition(pos, probabilityOfBeep);
        }else{
            ship.scaleGridPosition(pos, 1-probabilityOfBeep);
        }

        sumProbabilities+=ship.getProbabilityAt(pos);
    }


    ship.normalizeProbabilities(sumProbabilities);


}




void ProbabilisticBot::moveToNextLocation(Ship& ship){
    if(id == "bot9"){
        intelligentStep(ship);
    }else{
        naiveNextStep(ship);
    }

}

void ProbabilisticBot::intelligentStep(Ship& ship){
    std::vector<std::pair<int, int>> candidates;

    if(ship.almostDone()){
        candidates = ship.getIntelligentStep(currentPosition);
    }else{
        candidates = ship.getMostProbable(currentPosition, openPositions);
    }


    std::pair<int, int> nextPosition = Utility::shufflePositions(candidates);
    std::vector<std::pair<int, int>> pathToNextPosition = ship.getShortestPath(currentPosition, nextPosition);
    bool foundLeak = false;
    for(const auto& pos : pathToNextPosition){
        currentPosition = pos;
        if(ship.canPlugLeak(currentPosition)){
            if(ship.plugLeak(currentPosition) && !ship.hasLeaks()){
                if(!ship.hasLeaks()){
                    active = false;
                    return;
                }else{  // we change to using bayesian instead of normal matrix

                    foundLeak = true;
                }

            }
        }
        if(foundLeak){
            // remove double probability not containing current
            ship.removeInversePossibility(currentPosition);
        }else{
            // remove double probability
            ship.removePossibility(currentPosition);
        }

        //ship.normalizePairs(openPositions);// no longer needed, I am doing this inside of remove now // this can difinitely be optimized; also might have to change how you normalize; check when not drunk

        // remore single probability
        Utility::removePosition(openPositions, currentPosition);
        path.emplace_back(pos);

        // update normal probabilities
        float prob = ship.getProbabilityAt(currentPosition);
        float norm = prob/openPositions.size();     // this doesn't feel right; double check real quick
        ship.updateProbabilities(currentPosition, norm);

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
        float prob = ship.getProbabilityAt(currentPosition);
        float norm = prob/openPositions.size();
        ship.updateProbabilities(currentPosition, norm);
        totalActions+=1;
    }

}
