#include "../headers/BayesianPairs.h"

BayesianPairs::BayesianPairs():size(0)
{
}

void BayesianPairs::addPair(const std::pair<int, int>& c1, const std::pair<int, int>& c2){
    Coordinate first = {c1.first, c1.second};
    Coordinate second = {c2.first, c2.second};

    if(hash_map.find({first, second}) == hash_map.end()){
        std::pair<Coordinate, Coordinate> key = {first, second};
        hash_map[key] = 0.0;
        size++;
    }

}
void BayesianPairs::setProbability(const std::pair<int, int>& c1, const std::pair<int, int>& c2, float modifier){
    Coordinate first = {c1.first, c1.second};
    Coordinate second = {c2.first, c2.second};

    if(hash_map.find({first, second}) != hash_map.end()){
        hash_map[{first, second}] = modifier;
    }
}

void BayesianPairs::scalePairs(const std::pair<int,int>& x, const std::pair<int, int>& y, float mod){
    Coordinate first = {x.first, x.second};
    Coordinate second = {y.first, y.second};
    if(hash_map.find({first, second}) != hash_map.end()){
        hash_map[{first, second}] *= mod;
    }
}

void BayesianPairs::normalize(float factor){
    for(auto& pair : hash_map){
        pair.second /= factor;
    }
}

std::vector<std::pair<int, int>> BayesianPairs::getHighestProbabilities(const std::pair<int, int>& current){
    std::vector<std::pair<int, int>> statement;
    float highestProbability = 0.0;

    for(const auto& pair : hash_map){
        if(pair.second > highestProbability){
            highestProbability = pair.second;
        }
    }

    for(const auto& pair : hash_map){
        if(pair.second == highestProbability){
            std::pair<int, int> probable(pair.first.second.x, pair.first.second.y);
            statement.emplace_back(probable);  // we want a list of the second position in the key
        }
    }

    return statement;

}

float BayesianPairs::get(const std::pair<int, int>& i, const std::pair<int, int>& j){
    Coordinate first = {i.first, i.second};
    Coordinate second = {j.first, j.second};
    if(hash_map.find({first, second}) != hash_map.end()){
        return hash_map[{first, second}];
    }

    return 0.0;
}

void BayesianPairs::remove(const std::pair<int, int>& current){

    // remove all positions of the form {current, ___}; at every pair i, redistribute the probability after erasing
    float combined_prob_removed =0.0;
    Coordinate target = {current.first, current.second};

    for(auto it = hash_map.begin(); it != hash_map.end();){
        // it is { key, value}, key is {coord, coord}
        if(it->first.first == target){
            combined_prob_removed += it->second;
            it = hash_map.erase(it);
            size--;
        }else if(it->first.second == target){
            combined_prob_removed += it->second;
            it = hash_map.erase(it);
            size--;
        }else{
            ++it;
        }
    }

    float modifier = combined_prob_removed/size;
    redistribute(modifier);


}

void BayesianPairs::secondPositionRemove(const std::pair<int, int>& current){
    float combined_prob_removed =0.0;
    Coordinate target = {current.first, current.second};

    for(auto it = hash_map.begin(); it != hash_map.end();){
        // it is { key, value}, key is {coord, coord}
        if(it->first.second == target){
            combined_prob_removed += it->second;
            it = hash_map.erase(it);
            size--;
        }else{
            ++it;
        }
    }

    float modifier = combined_prob_removed/size;
    redistribute(modifier);
}

void BayesianPairs::narrowDownSearchSpace(const std::pair<int, int>& current){
    float combined_prob_removed =0.0;
    Coordinate goal = {current.first, current.second};

    for(auto it = hash_map.begin(); it != hash_map.end();){
        // it is { key, value}, key is {coord, coord}
        if(it->first.first != goal){
            combined_prob_removed += it->second;
            it = hash_map.erase(it);
            size--;
        }else{
            ++it;
        }
    }

    float modifier = combined_prob_removed/size;
    redistribute(modifier);
}

void BayesianPairs::redistribute(float modifier){

    for(auto& pair : hash_map){
        pair.second+=modifier;
    }
}

void BayesianPairs::updatePairProbability(Table& distances, Sensor& sensor,const std::pair<int, int>& curr, bool signalDetected){
    float normFactor = 0.0;
    for(auto& pair: hash_map){
        std::pair<int, int> i(pair.first.first.x, pair.first.first.y);
        std::pair<int, int> j(pair.first.second.x, pair.first.second.y);
        int dist_ki = distances.getDistance(curr, i);
        int dist_kj = distances.getDistance(curr, j);

        float P_beep_ki = sensor.getProbability(dist_ki);
        float P_beep_kj = sensor.getProbability(dist_kj);
        float P_given_ij = P_beep_ki*P_beep_kj;

        float probabilityOfBeep = P_beep_ki + P_beep_kj - P_given_ij;

        if(signalDetected){
            pair.second += probabilityOfBeep;
        }else{
            pair.second += (1-probabilityOfBeep);
        }

        normFactor+=pair.second;
    }

    normalize(normFactor);
}

void BayesianPairs::lateGameUpdate(Table& distances, Sensor& sensor, const std::pair<int, int>& curr, bool signalDetected){
    float normFactor = 0.0;
    for(auto& pair : hash_map){
        std::pair<int, int> second(pair.first.second.x, pair.first.second.y);
        int distance = distances.getDistance(curr, second);
        float probOfBeep = sensor.getProbability(distance);
        if(signalDetected){
            pair.second += probOfBeep;
        }else{
            pair.second += (1-probOfBeep);
        }

        normFactor += pair.second;
    }

    normalize(normFactor);
}

void BayesianPairs::updateBeliefTable(Belief& belief_table){
    for(auto& pair : hash_map){
        belief_table.updateBelief(pair.first.second, pair.second);
    }
}
