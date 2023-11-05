#include "../headers/Ship.h"
#include <algorithm> // For std::shuffle
#include <random>    // For std::default_random_engine
#include <chrono>    // For std::chrono::system_clock


#define OPEN 0
#define CLOSED 1


Ship::Ship(int size) : dimensions(size), grid(size, std::vector<int>(size, 0)),
                       rng(std::chrono::system_clock::now().time_since_epoch().count()) { // Seeding the random number generator
    initializeGrid();
}

Ship::~Ship() {
    // No dynamic allocation used, so no need for a custom destructor
}

void Ship::initializeGrid() {
    // we use closed=0 and open=-1 for grid at the beginning.
    // Initialize the grid as a DxD blocked cells (blocked means 0)
    grid = std::vector<std::vector<int>>(dimensions, std::vector<int>(dimensions, 0));

    // Randomly choose a cell and open it
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
        }else if (grid[position.first][position.second] != 0) {
            isPositionValid = false;
        }

    } while (!isPositionValid);

    // If we have a valid position that's open, set it as a leak on the grid and add the Leak
    grid[position.first][position.second] = 2; // Mark the position as having a leak
    leaks.emplace_back(position); // Add the new Leak object with the position
}



// bool Ship::isLeakAt(int x, int y) {
//     return x >= 0 && x < dimensions && y >= 0 && y < dimensions && grid[x][y] == 1;
// }

// void Ship::plugLeakAt(int x, int y) {
//     if (isLeakAt(x, y)) {
//         grid[x][y] = 0; // Set the cell to 0 to indicate the leak has been plugged
//         // Find the leak in the leaks vector and remove it
//         leaks.erase(std::remove_if(leaks.begin(), leaks.end(), [x, y](const Leak& leak) {
//             return leak.getX() == x && leak.getY() == y;
//         }), leaks.end());
//     }
// }

std::vector<std::pair<int, int>> Ship::getNeighbors(int x, int y) const {
    std::vector<std::pair<int, int>> neighbors;
    
    // Adjust the conditions to exclude perimeter positions
    if (x > 1) neighbors.emplace_back(x - 1, y);           // Exclude left perimeter
    if (y > 1) neighbors.emplace_back(x, y - 1);           // Exclude top perimeter
    if (x < dimensions - 2) neighbors.emplace_back(x + 1, y); // Exclude right perimeter
    if (y < dimensions - 2) neighbors.emplace_back(x, y + 1); // Exclude bottom perimeter
    
    return neighbors;
}


std::vector<std::pair<int, int>> Ship::getOpenNeighbors(int x, int y) {
    std::vector<std::pair<int, int>> openNeighbors;
    auto neighbors = getNeighbors(x, y);
    for (auto& n : neighbors) {
        if (grid[n.first][n.second] == 0) {
            openNeighbors.push_back(n);
        }
    }
    return openNeighbors;
}

std::vector<std::vector<int>> Ship::getGrid() const {
    // Actual implementation that returns the grid.
    // For example:
    return this->grid; // Assuming 'grid' is a class member of type std::vector<std::vector<int>>
}


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

std::set<std::pair<int, int>> Ship::get_dead_ends() {
    std::set<std::pair<int, int>> dead_ends;
    for (int i = 1; i < dimensions - 1; ++i) {
        for (int j = 1; j < dimensions - 1; ++j) {
            if (grid[i][j] == OPEN && is_dead_end(i, j)) {
                // Assuming get_closed_neighbors() is a method that returns closed neighbors
                auto closed_neighbors = get_closed_neighbors(i, j);
                for (const auto& neighbor : closed_neighbors) {
                    dead_ends.insert(neighbor);
                }
            }
        }
    }
    return dead_ends;
}

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

std::vector<std::pair<int, int>> Ship::getPositionOfLeaks() {
    std::vector<std::pair<int, int>> positions;
    for (const auto& leak : leaks) {
        positions.push_back(leak.getPosition());
    }
    return positions;
}


