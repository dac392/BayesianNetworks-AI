#include "../headers/Probability.h"
#include "../headers/Utility.h"

#define SUBSECTION_LENGTH 10

Probability::Probability(int dimensions) : dimensions(dimensions), belief_table()
{

}

/**
 * initiates probability based information and if bot8 or bot9 also initiates pair probability network
*/
void Probability::init(const std::vector<std::pair<int, int>>& open, bool needsNetwork){
    probability_matrix= std::vector<std::vector<float>>(dimensions, std::vector<float>(dimensions, 0));

    float totalOpen = open.size();
    float universal = 1/totalOpen;
    for(const auto& pos: open){
        probability_matrix[pos.first][pos.second] = universal;
        belief_table.updateBelief({pos.first, pos.second}, universal);
    }


    if(needsNetwork){
        float total_pairs = 0.0;
        for(const auto& c1 : open){
            for(const auto& c2 : open){
                if(c1 != c2){
                    table.addPair(c1, c2);
                    total_pairs++;
                }
            }
        }

        float modifier = 1/total_pairs;
        for(const auto& c1 : open){
            for(const auto& c2 : open){
                if(c1 != c2){
                    table.setProbability(c1, c2, modifier);
                }
            }
        }
    }
}

bool Probability::positionIsOpen(int i , int j){
    return !probability_matrix[i][j];
}

std::vector<std::vector<float>> Probability::getProbabilities(){
    return probability_matrix;
}

/**
 * used for finding the most favorable position in a 10x10 section
*/
std::pair<int, int> Probability::getPreferedPosition(const std::pair<int, int>& position){
    std::pair<int, int> favorable = belief_table.getFavoritePositions(position);

    int indexOfPos = belief_table.coordinateToIndex({favorable.first, favorable.second});
    float max_val = 0.0f;
    std::pair<int, int> goal = favorable;


    for(int i = 0; i < dimensions; i++){
        for(int j = 0; j < dimensions; j++){
            if(belief_table.coordinateToIndex({i, j}) == indexOfPos && probability_matrix[i][j] > max_val){
                max_val = probability_matrix[i][j];
                goal = std::pair<int, int>(i, j);
            }
        }
    }

    return goal;

}

/**
 * simple probability update for single leak and bot7
*/
void Probability::simpleHeuristicUpdate(){
    belief_table.resetBelief();
    for(int i = 0; i < dimensions; i++){
        for(int j = 0; j < dimensions; j++){
            if(probability_matrix[i][j]>0){
                belief_table.updateBelief({i, j}, probability_matrix[i][j]);
            }
        }
    }
}

/**
 * updates pair proabilities
*/
void Probability::harderHeuristicUpdate(){
    belief_table.resetBelief();
    table.updateBeliefTable(belief_table);
}


std::vector<std::pair<int, int>> Probability::getHighestProbabilityList() {
    std::vector<std::pair<int, int>> highestProbPositions;
    float highestProbability = 0.0;

    // Find the highest probability value in the matrix
    for (int i = 0; i < dimensions; ++i) {
        for (int j = 0; j < dimensions; ++j) {
            if (probability_matrix[i][j] > highestProbability) {
                highestProbability = probability_matrix[i][j];
            }
        }
    }

    // Now find all positions with the highest probability
    for (int i = 0; i < dimensions; ++i) {
        for (int j = 0; j < dimensions; ++j) {
            if (probability_matrix[i][j] == highestProbability) {
                highestProbPositions.emplace_back(i, j);
            }
        }
    }

    return highestProbPositions;
}

void Probability::updateProbabilities(const std::pair<int, int>& pos, float norm){
    probability_matrix[pos.first][pos.second] = 0;
    for(int i = 0; i < dimensions; i++){
        for(int j = 0; j < dimensions; j++){
            if(probability_matrix[i][j]!=0.0f){
                probability_matrix[i][j]+=norm;
            }

        }
    }
}

void Probability::normalizeProbabilities(float modifier){
    belief_table.resetBelief();
    for(int i = 0; i < dimensions; i++){
        for(int j = 0; j < dimensions; j++){
            if(positionIsOpen(i, j)){
                probability_matrix[i][j] /= modifier;
                belief_table.updateBelief({i, j}, probability_matrix[i][j]);
            }
        }
    }
}

void Probability::normalizePairs(float factor){
    table.normalize(factor);
}

float Probability::getProbabilityAt(const std::pair<int, int>& pos){
    return probability_matrix[pos.first][pos.second];
}

void Probability::scaleProbabilities(const std::pair<int, int>& pos, float modifier){
    probability_matrix[pos.first][pos.second]*=modifier;
}


void Probability::scaleBayesianPair(const std::pair<int,int>& x, const std::pair<int, int>& y, float mod){
    table.scalePairs(x, y, mod);
}

std::vector<std::pair<int, int>> Probability::getIntelligentStep(std::pair<int, int> current){
    return table.getHighestProbabilities(current);

}

void Probability::remove(const std::pair<int, int>& current){
    table.remove(current);
}

void Probability::smallerRemove(const std::pair<int, int>& current){
    table.secondPositionRemove(current);
}

void Probability::narrowDownSearchSpace(const std::pair<int, int>& current){
    table.narrowDownSearchSpace(current);
}

float Probability::getPairProbabilityAt(const std::pair<int, int>& i, const std::pair<int, int>& j){
    return table.get(i, j);
}

void Probability::updatePairProbability(Table& distances, Sensor& sensor, const std::pair<int, int>& curr, bool signalDetected){
    table.updatePairProbability(distances, sensor, curr, signalDetected);
}

void Probability::lateGameUpdate(Table& distances, Sensor& sensor, const std::pair<int, int>& curr, bool signalDetected){
    table.lateGameUpdate(distances, sensor, curr, signalDetected);
}

