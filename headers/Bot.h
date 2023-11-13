#ifndef BOT_H
#define BOT_H

#include "Sensor.h"
#include "Ship.h"
#include <string>
#include <utility>
#include <vector>

class Bot {
protected:
    bool dumb;
    bool active;
    int totalActions;
    std::string id;
    std::pair<int, int> initialPosition;
    std::pair<int, int> currentPosition;
    std::vector< std::pair<int, int> > openPositions;
    std::vector<std::pair<int, int>> goalPositions;
    std::vector<std::pair<int, int>> path;
    std::vector<std::pair<int, int>> pointsWhereIScanned;
    Sensor sensor;

    // Protected constructor to be called by derived classes
    Bot(const std::pair<int, int>& startPos, int range_mod, int alpha, const std::string& id, bool dumb)
        : currentPosition(startPos), initialPosition(startPos), sensor(range_mod, alpha), id(id), dumb(dumb) {
        active = true;
        totalActions=0;

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



    // Still pure virtual as the implementation is likely to be different in derived classes
    virtual void moveToNextLocation(Ship& ship) = 0;
    virtual std::string getType() = 0;

    virtual bool performScan(Ship& ship)=0;
    virtual void performDetected(Ship& ship)=0;
    virtual void performNotDetected(Ship& ship)=0;

    virtual void setLeakPositions(std::vector<std::pair<int, int>> positions){
        if(!positions.empty())
            goalPositions = positions;
    }

    // Accessor methods can be implemented here as they're likely to be common
    virtual std::pair<int, int> getCurrentPosition() const {
        return currentPosition;
    }

    virtual Sensor& getSensor() {
        return sensor;
    }

    virtual std::vector<std::pair<int, int>> getGoalPositions(){
        return goalPositions;
    }

    virtual std::vector<std::pair<int, int>>& getOpenPositions(){
        return openPositions;
    }

    virtual std::string getID(){
        return id;
    }

    virtual std::pair<int, int> getInitialPosition(){
        return initialPosition;
    }

    virtual std::vector<std::pair<int, int>> getPathTaken(){
        return path;
    }
    virtual std::vector<std::pair<int, int>> getPointsScanned(){
        return pointsWhereIScanned;
    }

    virtual int getTotalDistance(){
        return totalActions;
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
