#include "../headers/App.h"
#include <iostream>

App::App(int shipSize, int range_mod, int alpha) : environment(shipSize, range_mod, alpha) {
    environment.addBot("deterministic", "bot1", true);    // is dumb
    environment.addBot("deterministic", "bot2", true);    // is dumb
    // environment.addBot("probabilistic", "bot3", true);      // is dumb
    // environment.addBot("deterministic", "bot5", false);   // is not dumb
    // environment.addBot("probabilistic", "bot7", false);   // is not dumb
    // environment.addBot("probabilistic", "bot8", false);     // is not dumb
//    environment.addBot("probabilistic", "bot9", false);     // is not dumb
    
}

void App::printResults(){
    std::cout << "bot1 distance traveled: "<< environment.getDistanceFor("bot1") << std::endl;
    std::cout << "bot2 distance traveled: "<< environment.getDistanceFor("bot2") << std::endl;
    // std::cout << "bot3 distance traveled: "<< environment.getDistanceFor("bot3") << std::endl;
    // std::cout << "bot5 distance traveled: "<< environment.getDistanceFor("bot5") << std::endl;
    // std::cout << "bot7 distance traveled: "<< environment.getDistanceFor("bot7") << std::endl;
    // std::cout << "bot8 distance traveled: "<< environment.getDistanceFor("bot8") << std::endl;
}

void App::run() {
    environment.runSimulation();
    //environment.collectData();
    printResults();

}

int main() {
    int range_mod = 2;
    float alpha = 0.7f;
    App app(50, range_mod, alpha);
    app.run();
    return 0;
}
