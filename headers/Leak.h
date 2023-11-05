#ifndef LEAK_H
#define LEAK_H

#include <vector>

class Leak {
private:
    int x; // x coordinate of the leak's location
    int y; // y coordinate of the leak's location
    bool state; // true if the leak is active, false if it has been plugged

public:
    Leak(int x, int y); // Constructor that initializes the location and state of the leak
    ~Leak(); // Destructor

    // Accessor methods
    int getX() const; // Returns the x coordinate of the leak
    int getY() const; // Returns the y coordinate of the leak
    bool isActive() const; // Returns true if the leak is active

    // Mutator method
    void plug(); // Changes the state of the leak to inactive (plugged)
};

#endif // LEAK_H
