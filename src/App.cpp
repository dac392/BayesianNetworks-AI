#include "../headers/App.h"
#include <iostream>

App::App(int shipSize, int range_mod, int alpha) : environment(shipSize, range_mod, alpha) {
//    environment.addBot("deterministic", "bot1", true);    // is dumt
//    environment.addBot("probabilistic", "bot3", true);      // is dumb
//    environment.addBot("deterministic", "bot5", false);   // is not dumb
    environment.addBot("probabilistic", "bot7", false);   // is not dumb
    environment.addBot("probabilistic", "bot8", false);     // is not dumb
//    environment.addBot("probabilistic", "bot9", false);     // is not dumb
    
}

void App::run() {
    // Start the main logic of the application
    // This could include setting up the environment, adding bots, and running the simulation
    environment.runSimulation();
//    std::cout << "bot1 distance traveled: "<< environment.getDistanceFor("bot1") << std::endl;
//    std::cout << "bot3 distance traveled: "<< environment.getDistanceFor("bot3") << std::endl;
//    std::cout << "bot5 distance traveled: "<< environment.getDistanceFor("bot5") << std::endl;
    std::cout << "bot7 distance traveled: "<< environment.getDistanceFor("bot7") << std::endl;
    std::cout << "bot8 distance traveled: "<< environment.getDistanceFor("bot8") << std::endl;
}

// In your main program, you would typically create an instance of App and call run
int main() {
    int range_mod = 2;      // has to be greater than 1
    int alpha = 1; // has to be greater than 0
    App app(50, range_mod, alpha); // Initialize the App with a ship size of 10, for example
    app.run(); // Run the application
    return 0;
}
