#include "../headers/Spiral.h"
#include "../headers/Utility.h"
#include <algorithm>
#include <limits>
#include <iostream>

#define CLOSED 0
#define OPEN 1
#define DIRECTIONS 4

Spiral::Spiral(int k, const std::pair<int, int>& position) : k(k), position(position){
    detected = false;
    direction = 0;
    step_size = k;
    steps_taken = 0;

    dy = { 1, 1,-1, -1};
    dx = {-1, 1, 1, -1};

}

std::pair<int, int> Spiral::findNextPosition(Ship& ship, std::vector<std::pair<int, int>>& open) {
    if(detected){
        return informedSearch(ship, open);
    }

    return blindSearch(ship, open);

}

std::pair<int, int> Spiral::blindSearch(Ship& ship, std::vector<std::pair<int, int>>& open){
    if (open.empty()) {
        Utility::error("Fatal error: blind search, open is empty");
        return position; // Return current position if no open positions are available
    }

    std::vector<std::pair<int, int>> candidates = ship.getClosestReachable(position, open);

    for(int dead_end = 0; dead_end < DIRECTIONS; dead_end++){
        int modifier = 2*k;
        int x = position.first+(dx[direction]*modifier);
        int y = position.second+(dy[direction]*modifier);
        std::pair<int, int> goal(x, y);
        std::pair<int, int> op1(x+dx[direction], y-dy[direction]);
        std::pair<int, int> op2(x-dx[direction], y+dy[direction]);

        if(std::find(open.begin(), open.end(), goal) != open.end()){
            position = goal;
            funky_update();
            return goal;
        }
        if(std::find(open.begin(), open.end(), op1) != open.end()){
                position = op1;
                funky_update();
                return op1;
        }
        if(std::find(open.begin(), open.end(), op2) != open.end()){
                position = op2;
                funky_update();
                return op2;
        }



    }

    std::pair<int, int> p = Utility::shufflePositions( candidates );
    position = p;
    funky_update();
    return p;
}



std::pair<int, int> Spiral::informedSearch(Ship& ship, std::vector<std::pair<int, int>>& open) {
    if(search(open, 0)){
        return position;
    }
    std::vector<std::pair<int, int>> candidates = ship.getClosestReachable(position, open);
    // auto& list = (candidates.empty())? open : candidates;
    std::pair<int, int> next;
    do{
        next = Utility::shufflePositions( candidates );
    }while(position == next);
     
    position = next;
    return position;
}

bool Spiral::search(const std::vector<std::pair<int, int>>& open, int dead_end){
    if(dead_end == 4){
        return false;
    }

    int x = position.first + dx[direction];
    int y = position.second + dy[direction];
    std::pair<int, int> p(x, y);
    if(std::find(open.begin(), open.end(), p)!=open.end()){
        position.first+=dx[direction];
        position.second+=dy[direction];
        update();
        return true;
    }

    reverse_update();
    dead_end++;
    return search(open, dead_end);

}




void Spiral::adapt(){
    detected = true;
    dx = {0, 1, 0, -1};
    dy = {-1, 0, 1, 0};
    step_size = 2*k;
}









// helper function

void Spiral::update(){
    direction = (direction + 1) % 4;
}

void Spiral::funky_update(){

    if (steps_taken == step_size) {
        // Change direction
        direction = (direction + 1) % 4;
        steps_taken = 0;

        // Every two direction changes, increase the steps in the layer
        if (direction % 2 == 0) {
            step_size += 2 * k;
        }
    }

    steps_taken++;
}

void Spiral::reverse_update(){
    
    direction = (direction - 1) % 4;

}
