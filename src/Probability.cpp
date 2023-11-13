#include "../headers/Probability.h"

Probability::Probability(int dimensions) : dimensions(dimensions)
{

}

void Probability::init(const std::vector<std::pair<int, int>>& open, bool needsNetwork){
    probability_matrix= std::vector<std::vector<float>>(dimensions, std::vector<float>(dimensions, 0));

    float totalOpen = open.size();
    float universal = 1/totalOpen;
    for(const auto& pos: open){
        probability_matrix[pos.first][pos.second] = universal;
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
            if(probability_matrix[i][j]!=0){
                probability_matrix[i][j]+=norm;
            }

        }
    }
}

void Probability::normalizeProbabilities(float modifier){
    for(int i = 0; i < dimensions; i++){
        for(int j = 0; j < dimensions; j++){
            if(positionIsOpen(i, j)){
                probability_matrix[i][j] /= modifier;
            }
        }
    }
}

void Probability::normalizePairs(const std::vector<std::pair<int, int>>& open, float factor){
    for(const auto& i : open){
        for(const auto& j : open){
            if(i!=j){   // don't have to check if they'er closed
                table.normalize(i, j, factor);
            }
        }
    }
}

float Probability::getProbabilityAt(std::pair<int, int>& pos){
    return probability_matrix[pos.first][pos.second];
}

void Probability::scaleProbabilities(std::pair<int, int>& pos, float modifier){
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

void Probability::narrowDownSearchSpace(const std::pair<int, int>& current){
    table.narrowDownSearchSpace(current);
}

//void Probability::normalizePairs(const std::vector<std::pair<int, int>>& open){
//    table.normalizePairs(open);
//}

float Probability::getPairProbabilityAt(const std::pair<int, int>& i, const std::pair<int, int>& j){
    return table.get(i, j);
}
