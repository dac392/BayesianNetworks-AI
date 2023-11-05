#ifndef LEAK_H
#define LEAK_H

#include <vector>

class Leak {
private:
    std::pair<int, int> position;
    bool state; // true if the leak is active, false if it has been plugged

public:
    Leak(std::pair<int, int> position); // Constructor that initializes the location and state of the leak
    ~Leak(); // Destructor

    // Accessor methods
    std::pair<int,int> getPosition() const;
    bool isActive() const; // Returns true if the leak is active

    // Mutator method
    void plug(); // Changes the state of the leak to inactive (plugged)
};

#endif // LEAK_H
