#ifndef LEAK_H
#define LEAK_H

#include <vector>

class Leak {
private:
    std::pair<int, int> position;
    bool state; 

public:
    Leak(std::pair<int, int> position); 
    std::pair<int,int> getPosition() const;
    bool isActive() const;
    void plug(); 
    void reset();
};

#endif // LEAK_H
