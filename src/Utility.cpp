#include "Utility.h"
#include <cstdlib>
#include <ctime>

std::vector<int> Utility::generateCoordinate(){
    // Seed the random number generator.
    // Note: You normally only seed once, so this would go elsewhere in a full program.
    static bool seeded = false;
    if (!seeded) {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        seeded = true;
    }

    // Generate two random numbers between 1 and 48
    int x = std::rand() % 48 + 1;
    int y = std::rand() % 48 + 1;

    // Create a vector with the generated coordinates and return it
    std::vector<int> coordinate = {x, y};
    return coordinate;
}
