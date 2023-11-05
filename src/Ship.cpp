#include "../headers/Ship.h"
#include <algorithm> // For std::shuffle
#include <random>    // For std::default_random_engine
#include <chrono>    // For std::chrono::system_clock



Ship::Ship(int size) : dimensions(size), grid(size, std::vector<int>(size, 0)),
                       rng(std::chrono::system_clock::now().time_since_epoch().count()) { // Seeding the random number generator
    initializeGrid();
}

Ship::~Ship() {
    // No dynamic allocation used, so no need for a custom destructor
}

void Ship::initializeGrid() {
 // Initialize the grid as a DxD blocked cells
    grid = std::vector<std::vector<int>>(dimensions, std::vector<int>(dimensions, 1));

    // Randomly choose a cell and open it
    std::uniform_int_distribution<int> dis(1, dimensions - 2); // interior cells only
    int start_x = dis(rng);
    int start_y = dis(rng);
    grid[start_x][start_y] = 0;

    std::set<std::pair<int, int>> frontier;

    // Get initial frontier cells (blocked cells with exactly one open neighbor)
    auto initial_neighbors = getNeighbors(start_x, start_y);
    for (auto& n : initial_neighbors) {
        if (grid[n.first][n.second] == 1) {
            frontier.insert(n);
        }
    }

    // Iteratively open cells
    while (!frontier.empty()) {
        // Convert set to vector for random access
        std::vector<std::pair<int, int>> frontier_vec(frontier.begin(), frontier.end());

        // Randomly select a cell from the frontier
        std::shuffle(frontier_vec.begin(), frontier_vec.end(), rng);
        auto cell = frontier_vec.front();
        frontier.erase(cell);

        // Open the cell
        grid[cell.first][cell.second] = 0;

        // Update the frontier
        auto neighbors = getNeighbors(cell.first, cell.second);
        for (auto& n : neighbors) {
            if (grid[n.first][n.second] == 1 && getOpenNeighbors(n.first, n.second).size() == 1) {
                frontier.insert(n);
            }
        }
    }

    // Identify dead ends and open half of them randomly
    std::vector<std::pair<int, int>> deadEnds;
    for (int x = 1; x < dimensions - 1; ++x) {
        for (int y = 1; y < dimensions - 1; ++y) {
            if (grid[x][y] == 0 && getOpenNeighbors(x, y).size() == 1) {
                deadEnds.emplace_back(x, y);
            }
        }
    }

    std::shuffle(deadEnds.begin(), deadEnds.end(), rng);

    // Open half of the dead ends' neighbors
    for (size_t i = 0; i < deadEnds.size() / 2; ++i) {
        auto deadEnd = deadEnds[i];
        auto closedNeighbors = getNeighbors(deadEnd.first, deadEnd.second);
        closedNeighbors.erase(std::remove_if(closedNeighbors.begin(), closedNeighbors.end(), [this](const std::pair<int, int>& n) {
            return grid[n.first][n.second] == 0;
        }), closedNeighbors.end());

        if (!closedNeighbors.empty()) {
            std::shuffle(closedNeighbors.begin(), closedNeighbors.end(), rng);
            grid[closedNeighbors.front().first][closedNeighbors.front().second] = 0;
        }
    }
}

void Ship::placeLeak(int x, int y) {
    if (x >= 0 && x < dimensions && y >= 0 && y < dimensions) {
        grid[x][y] = 1; // Set the cell to 1 to indicate a leak
        leaks.push_back(Leak(x, y)); // Add a new Leak object to the vector of leaks
    }
}

bool Ship::isLeakAt(int x, int y) {
    return x >= 0 && x < dimensions && y >= 0 && y < dimensions && grid[x][y] == 1;
}

void Ship::plugLeakAt(int x, int y) {
    if (isLeakAt(x, y)) {
        grid[x][y] = 0; // Set the cell to 0 to indicate the leak has been plugged
        // Find the leak in the leaks vector and remove it
        leaks.erase(std::remove_if(leaks.begin(), leaks.end(), [x, y](const Leak& leak) {
            return leak.getX() == x && leak.getY() == y;
        }), leaks.end());
    }
}

std::vector<std::pair<int, int>> Ship::getNeighbors(int x, int y) {
    std::vector<std::pair<int, int>> neighbors;
    if (x > 0) neighbors.emplace_back(x - 1, y);
    if (y > 0) neighbors.emplace_back(x, y - 1);
    if (x < dimensions - 1) neighbors.emplace_back(x + 1, y);
    if (y < dimensions - 1) neighbors.emplace_back(x, y + 1);
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



