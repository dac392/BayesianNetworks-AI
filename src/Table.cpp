#include "../headers/Table.h"
#include <cmath>
#include <limits>
#include <queue>

Table::Table()
{

}

bool isValid(const std::vector<std::vector<int>>& grid, int x, int y) {
    return x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE && grid[x][y] == 0;
}



void Table::init(const std::vector<std::vector<int>>& grid){
    for(int i = 0; i < GRID_SIZE; i++){
        for(int j = 0; j < GRID_SIZE; j++){
            if(grid[i][j] != 1){
                bfs(grid, {i,j});
            }
        }
    }
}

/**
 * runs unbounded dfs to find all distances
*/
void Table::bfs(const std::vector<std::vector<int>>& grid, Coordinate start){
    const std::vector<std::pair<int, int>> DIRECTIONS = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    std::queue<Coordinate> q;
    q.push(start);

    std::unordered_map<Coordinate, int, single_hash> local_distances;
    local_distances[start] = 0;

    while (!q.empty()) {
        Coordinate current = q.front();
        q.pop();

        for (const auto& dir : DIRECTIONS) {
            Coordinate next = {current.x + dir.first, current.y + dir.second};

            if (isValid(grid, next.x, next.y) && local_distances.find(next) == local_distances.end()) {
                q.push(next);
                local_distances[next] = local_distances[current] + 1;
                hash_map[{start, next}] = local_distances[next];
            }
        }
    }
}

/**
 * retrieves distance from start to goal
*/
int Table::getDistance(const std::pair<int, int>& start, const std::pair<int, int>& goal){
    Coordinate from = {start.first, start.second};
    Coordinate to = {goal.first, goal.second};
    auto dist = hash_map.find({from, to});
    if(dist != hash_map.end()){
        return dist->second;
    }

    return -1;
}

/**
 * gets the closest open positon for some start position
*/
std::vector<std::pair<int, int>> Table::getClosestPositions(const std::pair<int, int>& start, const std::vector<std::pair<int, int>>& open){
    std::vector<std::pair<int, int>> closestPositions;
    int minimumDistance = std::numeric_limits<int>::max();
    for(const auto& pos : open){
        int distance = getDistance(start, pos);
        if (distance != -1 && distance <= minimumDistance){
            if (distance < minimumDistance) {
                closestPositions.clear();
                minimumDistance = distance;
            }
            closestPositions.push_back(pos);
        }
    }

    return closestPositions;
}

std::vector<std::pair<int, int>> Table::getPositionsInRange(const std::pair<int, int>& start, int range){

    std::vector<std::pair<int, int>> positions;

    for (int i = -range; i <= range; ++i) {
        for (int j = -range; j <= range; ++j) {
            // Calculate the new position
            int newI = start.first + i;
            int newJ = start.second + j;
            if(newI > 0 && newI<49 && newJ >0 && newJ <49 && std::abs(i) + std::abs(j) <= range){
                positions.push_back(std::make_pair(newI, newJ));
            }
        }
    }

    return positions;

}


