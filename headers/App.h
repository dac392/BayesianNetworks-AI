#ifndef APP_H
#define APP_H

#include "Environment.h"

class App {
private:
    Environment environment;

public:
    App(int shipSize); // Constructor that initializes the environment with a ship size
    void run(); // Starts the application
};

#endif // APP_H
