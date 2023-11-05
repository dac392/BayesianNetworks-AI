#include "App.h"
#include "DeterministicBot.h"
#include "ProbabilisticBot.h"

App::App(int shipSize) : environment(shipSize) {
    // The environment can be initialized here with bots or other settings if needed
    // For example:
    // environment.addBot(std::make_unique<DeterministicBot>());
    // environment.addBot(std::make_unique<ProbabilisticBot>());
}

void App::run() {
    // Start the main logic of the application
    // This could include setting up the environment, adding bots, and running the simulation
    environment.runSimulation();
}

// In your main program, you would typically create an instance of App and call run
int main() {
    App app(50); // Initialize the App with a ship size of 10, for example
    app.run(); // Run the application
    return 0;
}
