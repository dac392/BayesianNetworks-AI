#ifndef BOT_H
#define BOT_H

#include "Sensor.h"
#include <string>
#include <utility>
#include <vector>

class Bot {
protected:
    bool dumb;
    bool active;
    std::string id;
    std::pair<int, int> currentPosition;
    std::vector< std::pair<int, int> > openPositions;
    Sensor sensor;

    // Protected constructor to be called by derived classes
    Bot(const std::pair<int, int>& startPos, int range_mod, int alpha, const std::string& id, bool dumb)
        : currentPosition(startPos), sensor(range_mod, alpha), id(id), dumb(dumb) {
            active = true;
        }

public:
    virtual ~Bot() = default;

    // Common method implementations can be moved here if they're identical across derived classes
    virtual void move(int x, int y) {
        currentPosition = {x, y};
    }

    virtual bool isActive(){
        return active;
    }

    virtual bool isDumb(){
        return dumb;
    }

    virtual std::string getType() = 0;

    // Still pure virtual as the implementation is likely to be different in derived classes
    virtual bool scan(std::vector<std::pair<int, int>> leaks) = 0;
    
    // The same for performAction, it will be specific to each derived class
    virtual void performAction() = 0;

    // Accessor methods can be implemented here as they're likely to be common
    virtual std::pair<int, int> getCurrentPosition() const {
        return currentPosition;
    }

    virtual Sensor& getSensor() {
        return sensor;
    }

    // If populateOpenPositions is common, it can be implemented here
    virtual void populateOpenPositions(const std::vector<std::vector<int>>& grid) {
        openPositions.clear();

        for (int i = 0; i < grid.size(); ++i) {
            for (int j = 0; j < grid[i].size(); ++j) {
                if (grid[i][j] == 0 && !(currentPosition.first == i && currentPosition.second == j)) {
                    openPositions.emplace_back(i, j);
                }
            }
        }
    }
};

#endif // BOT_H
