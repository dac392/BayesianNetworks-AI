#include "../headers/App.h"
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <atomic> // Include for std::atomic

#define EXPERIMENT_NUMBER 20

bool otherThreadsCompleted = false;
std::mutex dataMutex;
std::mutex spinMutex;
std::condition_variable spinCondition;
std::atomic<bool> stopSpinningThread(false); // Atomic flag to signal spinning thread


void spinThread(int range_mod, float alpha) {
    while (!stopSpinningThread.load()) { // Check the flag in each iteration
        App app(50, range_mod, alpha);
        app.run();
        dataMutex.lock();
        app.collectData();
        dataMutex.unlock();

        // Sleep for 30 seconds
        std::cout << "Thread sleeping for 30 seconds..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(30));
        std::cout << "Thread woke up!" << std::endl;
    }
}

void simpleThread(int range_mod, float alpha){
        App app(50, range_mod, alpha);
        app.run();
        dataMutex.lock();
        app.collectData();
        dataMutex.unlock();
}

void nonSpinThread(int range_mod, float alpha) {
    App app(50, range_mod, alpha);
    app.walk();
    dataMutex.lock();
    app.collectData();
    dataMutex.unlock();
}

int main() {
    // std::vector<std::thread> threads;

    // for (int i = 4; i < EXPERIMENT_NUMBER; i++) {
    //     int range_mod = 1 + (i % 10);
    //     float alpha = 0.05f * i + 0.05;
    //     for (int j = 0; j < 3; ++j) {
    //         threads.emplace_back(nonSpinThread, range_mod, alpha);
    //     }

    //     // Wait for non-spinning threads to finish
    //     for (auto& thread : threads) {
    //         thread.join();
    //     }

    //     std::thread simple(simpleThread, range_mod, alpha);
    //     simple.join();

    //     threads.clear();
    // }

    return 0;
}


App::App(int shipSize, int range_mod, int alpha) : environment(shipSize, range_mod, alpha) {
//    environment.addBot("probabilistic", "bot9", false);     // is not dumb
    
}

void App::printResults(){
    std::cout << "\tbot1 distance traveled: "<< environment.getDistanceFor("bot1") << std::endl;
    std::cout << "\tbot2 distance traveled: "<< environment.getDistanceFor("bot2") << std::endl;
    std::cout << "\tbot3 distance traveled: "<< environment.getDistanceFor("bot3") << std::endl;
    std::cout << "\tbot4 distance traveled: "<< environment.getDistanceFor("bot4") << std::endl;
    std::cout << "\tbot5 distance traveled: "<< environment.getDistanceFor("bot5") << std::endl;
    std::cout << "\tbot6 distance traveled: "<< environment.getDistanceFor("bot6") << std::endl;
    std::cout << "\tbot7 distance traveled: "<< environment.getDistanceFor("bot7") << std::endl;
    std::cout << "\tbot8 distance traveled: "<< environment.getDistanceFor("bot8") << std::endl;
    std::cout << "\tbot9 distance traveled: "<< environment.getDistanceFor("bot9") << std::endl;
}

void App::run() {
    // environment.addBot("deterministic", "bot1", true);    // is dumb
    // environment.addBot("deterministic", "bot2", true);
    // environment.addBot("probabilistic", "bot3", true);      // is dumb
    // environment.addBot("deterministic", "bot5", false);   // is not dumb
    // environment.addBot("deterministic", "bot6", false);    // is dumb

    environment.addBot("probabilistic", "bot4", true);
    environment.addBot("probabilistic", "bot9", false);

    environment.runSimulation();
    printResults();
}

void App::walk(){
    environment.addBot("probabilistic", "bot7", false);   // is not dumb
    environment.addBot("probabilistic", "bot8", false);     // is not dumb

    environment.runSimulation();
    printResults();
}

void App::collectData(){
    environment.collectData();
}

// void spinThread(int range_mod, float alpha, bool easy){
//     App app(50, range_mod, alpha);
//     if(easy){
//         app.run();
//     }else{
//         app.walk();
//     }

//     dataMutex.lock();
//     app.collectData();
//     dataMutex.unlock();

// }

int main() {
    std::vector<std::thread> threads;
    for(int i = 0; i < EXPERIMENT_NUMBER; i++){
        int range_mod = 1+(i%10);
        float alpha = 0.05f*i +0.05;
        App app(50, range_mod, alpha);
        app.run();
    }
    
    return 0;
}
