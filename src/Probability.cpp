#include "../headers/Probability.h"
#include "../headers/Utility.h"

#define SUBSECTION_LENGTH 10

Probability::Probability(int dimensions) : dimensions(dimensions), belief_table()
{

}

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

std::pair<int, int> Probability::getPreferedPosition(const std::pair<int, int>& position){
    std::vector<std::pair<int, int>> favorableList = belief_table.getFavoritePositions(position);
    if(favorableList.size() == 1 && favorableList[0]==position){
        int indexOfPos = belief_table.coordinateToIndex({position.first, position.second});
        std::pair<int, int> start_coord = belief_table.indexToCoordinate(indexOfPos);
        float max_val = 0.0f;
        std::pair<int, int> goal;
        for(int i = start_coord.first; i < SUBSECTION_LENGTH; i++){
            for(int j = start_coord.second; j < SUBSECTION_LENGTH; j++){
                if(probability_matrix[i][j] > max_val){
                    max_val = probability_matrix[i][j];
                    goal = std::pair<int, int>(i, j);
                }
            }
        }

        return goal;
    }


    std::pair<int, int> p = Utility::shufflePositions(favorableList);
    return p;
}
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
    belief_table.resetBelief();
    probability_matrix[pos.first][pos.second] = 0;
    for(int i = 0; i < dimensions; i++){
        for(int j = 0; j < dimensions; j++){
            if(probability_matrix[i][j]!=0){
                probability_matrix[i][j]+=norm;
                belief_table.updateBelief({i, j}, probability_matrix[i][j]);
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

