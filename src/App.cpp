#include "App.h"
#include "DeterministicBot.h"
#include "ProbabilisticBot.h"

App::App(int shipSize, int range_mod, int alpha) : environment(shipSize, range_mod, alpha) {
    environment.addBot("Deterministic", "bot1");
    
}

void App::run() {
    // Start the main logic of the application
    // This could include setting up the environment, adding bots, and running the simulation
    environment.runSimulation();
}

// In your main program, you would typically create an instance of App and call run
int main() {
    int range_mod = 2;      // has to be greater than 1
    int alpha = 1; // has to be greater than 0
    App app(50, range_mod, alpha); // Initialize the App with a ship size of 10, for example
    app.run(); // Run the application
    return 0;
}
