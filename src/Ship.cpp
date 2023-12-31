#include "../headers/Ship.h"
#include "../headers/Utility.h"
#include <algorithm> // For std::shuffle
#include <random>    // For std::default_random_engine
#include <chrono>    // For std::chrono::system_clock
#include <map>
#include <queue>
#include <stdexcept>
#include <utility>
#include <iostream>

#include <cmath>
#include <limits>


#define OPEN 0
#define CLOSED 1
#define EXPLORED 3

/**
 * Constructor
*/
Ship::Ship(int size) : dimensions(size), grid(size, std::vector<int>(size, 0)),
    rng(std::chrono::system_clock::now().time_since_epoch().count()) {
    initializeGrid();
    distanceTable.init(grid);

}

/**
 * resets leaks depenging on whether the bot can handle 1 or 2 leaks
*/
void Ship::fixLeaks(bool one_leak){
    if(one_leak){
        leaks[0].reset();
        leaks[1].plug();
    }else{
        leaks[0].reset();
        leaks[1].reset();
    }
}

void Ship::setID(const std::string& ship_name){
    id = ship_name;
}

std::string Ship::get_uid(){
    return id;
}

bool Ship::hasLeaks(){
    for(const auto& leak : leaks){
        if(leak.isActive()){
            return true;
        }
    }
    return false;
}

/**
 * used for sequential runs
 * only used for debugging purposes
*/
bool Ship::firstRoundTest(){
    if(leaks.empty())
        return true;

    return false;
}

/**
 * initializes the gameboard map
*/
void Ship::initializeGrid() {
    grid = std::vector<std::vector<int>>(dimensions, std::vector<int>(dimensions, 0));
    std::uniform_int_distribution<int> dis(1, dimensions - 2); // interior cells only
    int start_x = dis(rng);
    int start_y = dis(rng);

    std::vector<std::pair<int, int>> fringe;
    fringe.push_back({start_x, start_y});

    while (!fringe.empty()) {
        // Get a random coordinate from the fringe and name it curr
        std::shuffle(fringe.begin(), fringe.end(), rng);
        std::pair<int, int> curr = fringe.back();
        fringe.pop_back();

        // If the grid at the coordinate curr is closed,
        if (grid[curr.first][curr.second] == 0 || grid[curr.first][curr.second] == 1) {
            grid[curr.first][curr.second] = -1;
            auto neighbors = getNeighbors(curr.first, curr.second);
            for (auto& neighbor : neighbors) {
                if (grid[neighbor.first][neighbor.second] != -1) {
                    grid[neighbor.first][neighbor.second]++;
                    fringe.push_back(neighbor);
                }
            }
        }
    }
    normalizeGrid();
    auto dead_ends = get_dead_ends();
    eliminate_dead_ends(dead_ends);
}
bool contains(const std::vector<std::pair<int, int>>& vec, const std::pair<int, int>& element) {
    return std::find(vec.begin(), vec.end(), element) != vec.end();
}

/**
 * used for retreiving distance from distance table
*/
int Ship::getDistanceFrom(const std::pair<int, int>& start, const std::pair<int, int>& end){
    return distanceTable.getDistance(start, end);
}

/**
 * resets the game board for next bot to use
*/
void Ship::normalizeGrid() {
    for (int i = 0; i < dimensions; ++i) {
        for (int j = 0; j < dimensions; ++j) {
            if (grid[i][j] >= 0) {
                grid[i][j] = CLOSED;
            } else if (grid[i][j] == -1) {
                grid[i][j] = OPEN;
            }
        }
    }
}






/**
 * find closest reachable position from a start position
*/
std::vector<std::pair<int, int>> Ship::getClosestReachable(const std::pair<int, int>& start, const std::vector<std::pair<int, int>>& open) {
    return distanceTable.getClosestPositions(start, open);
}


/**
 * get most probable position from a start position
*/
std::vector<std::pair<int, int>> Ship::getMostProbable(const std::pair<int, int>& current, std::vector<std::pair<int, int>>& mostProbable) {
    return distanceTable.getClosestPositions(current, mostProbable);
}


/**
 * get shortest path from start to ending position
*/
std::vector<std::pair<int, int>> Ship::getShortestPath(std::pair<int, int> start, std::pair<int, int> goal) {
    std::queue<std::pair<int, int>> q;
    std::map<std::pair<int, int>, std::pair<int, int>> predecessors;
    std::vector<std::pair<int, int>> path;

    q.push(start);
    predecessors[start] = {-1, -1};

    while (!q.empty()) {
        auto current = q.front();
        q.pop();

        // Check if the goal has been reached
        if (current == goal) {
            // Construct the path by backtracking from the goal to the start
            while (current != start) {
                path.push_back(current);
                current = predecessors[current];
            }
            path.push_back(start);
            std::reverse(path.begin(), path.end());
            return path;
        }

        // Explore the neighbors of the current node
        auto neighbors = getOpenNeighbors(current.first, current.second);
        for (const auto& neighbor : neighbors) {
            // If the neighbor has not been visited
            if (predecessors.find(neighbor) == predecessors.end()) {
                q.push(neighbor);
                predecessors[neighbor] = current;
            }
        }
    }

    return path;
}


// Utility

bool Ship::positionIsOpen(int i, int j){
    return grid[i][j] !=CLOSED;
}

bool Ship::isPositionOpen(int i, int j, const std::vector<std::pair<int, int>>& open) {
    // Check if position is within the grid boundaries
    if (i < 0 || i >= Utility::cast(grid.size()) || j < 0 || j >= Utility::cast(grid[0].size())) return false;

    // Check if the position is not blocked and is in the open list
    if (grid[i][j] == 1) return false;

    for (const auto& pos : open) {
        if (pos.first == i && pos.second == j && (grid[i][j] == 0 || grid[i][j] == 2)) {
            return true;
        }
    }
    return false;
}

/**
 * resets ship information
*/
std::vector<std::pair<int, int>> Ship::reset() {
    std::vector<std::pair<int, int>> fixedLeaks;

    // Go through each leak and reset the grid position to 0
    for (auto& leak : leaks) {
        fixedLeaks.push_back(leak.getPosition()); // Add the leak position to the vector
        leak.reset();
    }

    for(int i = 0; i < dimensions; i++){
        for(int j = 0; j < dimensions; j++){
            if(grid[i][j]!=CLOSED && grid[i][j]!=OPEN){
                grid[i][j] = OPEN;
            }
        }
    }

    return fixedLeaks;
}

/**
 * mainly used for ship generation
*/
std::vector<std::pair<int, int>> Ship::getNeighbors(int x, int y) const {
    std::vector<std::pair<int, int>> neighbors;

    // Adjust the conditions to exclude perimeter positions
    if (x > 1) neighbors.emplace_back(x - 1, y);           // Exclude left perimeter
    if (y > 1) neighbors.emplace_back(x, y - 1);           // Exclude top perimeter
    if (x < dimensions - 2) neighbors.emplace_back(x + 1, y); // Exclude right perimeter
    if (y < dimensions - 2) neighbors.emplace_back(x, y + 1); // Exclude bottom perimeter

    return neighbors;
}

/**
 * mainly used for ship generation
*/
std::vector<std::pair<int, int>> Ship::getOpenNeighbors(int x, int y) {
    std::vector<std::pair<int, int>> openNeighbors;
    auto neighbors = getNeighbors(x, y);
    for (auto& n : neighbors) {
        if (grid[n.first][n.second] == 0 || grid[n.first][n.second] == 2) {
            openNeighbors.push_back(n);
        }
    }
    return openNeighbors;
}

/**
 * returns gameboard grid
*/
std::vector<std::vector<int>>& Ship::getGrid() {
    return grid;
}

/**
 * checks if a position contains a leak that can be plugged
*/
bool Ship::canPlugLeak(const std::pair<int, int>& pos){
    for(const auto& leak : leaks){
        if(leak.getPosition() == pos && leak.isActive()){
            return true;
        }
    }
    return false;
}

/**
 * mainly used for ship generation
*/
void Ship::eliminate_dead_ends(std::set<std::pair<int, int>>& dead_ends) {
    // Eliminate half of the dead ends randomly
    std::vector<std::pair<int, int>> dead_ends_vector(dead_ends.begin(), dead_ends.end());
    std::shuffle(dead_ends_vector.begin(), dead_ends_vector.end(), rng);

    int length_to_remove = dead_ends_vector.size() / 2;

    for (int i = 0; i < length_to_remove; ++i) {
        auto position = dead_ends_vector[i];
        grid[position.first][position.second] = OPEN; // Open up the dead end
        dead_ends.erase(position); // Remove from the dead ends set
    }
}


/**
 * add's new leak to the ship
*/
void Ship::addLeak(const std::vector<std::pair<int, int>>& blockedPositions) {
    std::pair<int, int> position;
    bool isPositionValid;

    do {
        position = Utility::generateCoordinate();
        isPositionValid = true;

        // Check against blocked positions
        if (std::find(blockedPositions.begin(), blockedPositions.end(), position) != blockedPositions.end()) {
            isPositionValid = false;

            // Check if the position on the grid is not 0 (which means it's not open for a new leak)
        }else if (grid[position.first][position.second] >= 1) {
            isPositionValid = false;
        }

    } while (!isPositionValid);

    leaks.emplace_back(position); // Add the new Leak object with the position
}

/**
 * mainly used for ship generation
*/
std::set<std::pair<int, int>> Ship::get_dead_ends() {
    std::set<std::pair<int, int>> dead_ends;
    for (int i = 1; i < dimensions - 1; ++i) {
        for (int j = 1; j < dimensions - 1; ++j) {
            if (grid[i][j] == OPEN && is_dead_end(i, j)) {
                auto closed_neighbors = get_closed_neighbors(i, j);
                for (const auto& neighbor : closed_neighbors) {
                    dead_ends.insert(neighbor);
                }
            }
        }
    }
    return dead_ends;
}

/**
 * mainly used for ship generation
*/
bool Ship::is_dead_end(int x, int y) const {
    std::vector<std::pair<int, int>> neighbors = getNeighbors(x, y);
    int open_count = 0;
    for (const auto& pos : neighbors) {
        if (grid[pos.first][pos.second] == OPEN) {
            ++open_count;
        }
    }
    return open_count == 1; // A dead end has exactly one open neighbor
}

/**
 * mainly used for ship generation
*/
std::vector<std::pair<int, int>> Ship::get_closed_neighbors(int x, int y) {
    std::vector<std::pair<int, int>> closed_neighbors;
    auto neighbors = getNeighbors(x, y);
    for (const auto& n : neighbors) {
        if (grid[n.first][n.second] != OPEN) {
            closed_neighbors.push_back(n);
        }
    }
    return closed_neighbors;
}

/**
 * returns the position of all leaks
*/
std::vector<std::pair<int, int>> Ship::getPositionOfLeaks() {
    std::vector<std::pair<int, int>> positions;
    for (const auto& leak : leaks) {
        positions.push_back(leak.getPosition());
    }
    return positions;
}

/**
 * deactivates a leak, returns bool based on whether there are more active leaks or not
*/
bool Ship::plugLeak(const std::pair<int, int>& pos){
    for(auto& l : leaks){
        if(pos == l.getPosition()){
            l.plug();
        }
    }

    for(const auto& l : leaks){
        if(l.isActive()){
            return false;   // if there are more leaks; return false
        }
    }

    return true;    // if there are no more leaks; return true
}

std::vector<Leak> Ship::getLeaks(){
    return leaks;
}


bool Ship::almostDone(){
    int count = 0;
    for(const auto& leak: leaks){
        if(leak.isActive()){
            count++;
        }
    }

    return count == 1;
}

Table& Ship::getDistances(){
    return distanceTable;
}
