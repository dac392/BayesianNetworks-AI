#ifndef SPIRAL_H
#define SPIRAL_H

#include "Ship.h"
#include <utility>
#include <vector>

class Spiral
{
private:
    bool detected;
    int k;
    int direction;
    int step_size;
    int steps_taken;

    std::vector<int> dx;
    std::vector<int> dy;
    std::pair<int, int> position;


    void update();
    void funky_update();
    void reverse_update();
    std::pair<int, int> blindSearch(Ship& ship, std::vector<std::pair<int, int>>& open);
    std::pair<int, int> informedSearch(Ship& ship, std::vector<std::pair<int, int>>& open);
    bool search(const std::vector<std::pair<int, int>>& open, int dead_end);

public:
    Spiral(int k, const std::pair<int, int>& curr);
    std::pair<int, int> findNextPosition(Ship& ship, std::vector<std::pair<int, int>>& open);
    void adapt();
};
#endif // SPIRAL_H
