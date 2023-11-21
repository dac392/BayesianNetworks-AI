#ifndef APP_H
#define APP_H

#include "Environment.h"

/**
 * used as driver for program execution
*/
class App {
private:
    int ff;
    Environment environment;
public:
    App(int ff, int shipSize, int range_mod, int alpha); // Constructor that initializes the environment with a ship size
    void run(); // Starts the application
    void printResults();
    void collectData();
    void walk();
};

#endif // APP_H
