#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>
#include <random>
#include <conio.h>
#include "Moore.h"

int main()
{
    Moore m;

    m.init();

    auto readAllInstr = [](Moore *m) {
        while (true)
        {
            m->updatePosition();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    };
    std::thread readT(readAllInstr, &m);
    readT.detach();

    std::cout << "[INFO] Position the machine at bottom right corner of test zone"  << std::endl;
    m.keyboardMove(true);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    m.defineZero();
    
    // Define the grid size and number of points per axis
    const double grid_size = 10.0;                                                // 10 mm
    const int points_per_axis = 3;                                                // 3 points for each axis
    const int total_points = points_per_axis * points_per_axis * points_per_axis; // Total positions

    // Create a vector to hold the positions
    std::vector<pos> positions;

    // Generate positions in a 3D grid
    for (int x = 0; x < points_per_axis; ++x)
    {
        for (int y = 0; y < points_per_axis; ++y)
        {
            for (int z = 0; z < points_per_axis; ++z)
            {
                positions.push_back({.x = x * (grid_size / (points_per_axis - 1)),
                                     .y = y * (grid_size / (points_per_axis - 1)),
                                     .z = z * (grid_size / (points_per_axis - 1))});
            }
        }
    }

    // Shuffle the positions
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(positions.begin(), positions.end(), g);

    // Open a CSV file to write the results
    std::ofstream csv_file("positions.csv");
    if (!csv_file.is_open())
    {
        std::cerr << "[ERROR] Error opening file!" << std::endl;
        return 1;
    }

    // Write the header
    csv_file << "Target_X,Target_Y,Target_Z,Machine_X,Machine_Y,Machine_Z\n";

    // Simulate the machine's target positions and write to CSV
    int i = 0;
    std::cout << "[INFO] Testing " << total_points << "points" << std::endl;
    for (const auto &target : positions)
    {
        m.setRelPosition(target);
        std::this_thread::sleep_for(std::chrono::seconds(5));
        pos actual = m.getRelPosition();
        // For this example, we assume the machine's position is the same as the target position
        csv_file << target << std::endl;
        csv_file << actual << std::endl;
        std::cout << "[INFO] " << i << " target: " << target << std::endl;
        std::cout << "[INFO] " << i << " actual: " << actual << std::endl;
        i++;
    }

    // Close the CSV file
    csv_file.close();

    std::cout << "Positions written to positions.csv" << std::endl;

    return 0;
}

/**
cd tests
g++ moore_test.cpp ../src/Moore.cpp ../src/Asse.cpp ../src/SimpleSerial.cpp ../src/Laser.cpp ../src/IkOptical.cpp ../src/Scale.cpp ../src/keysight.cpp ../src/pos.cpp ../src/cary.cpp -I../src/moore_inc -L../lib/gcc_dll -lni4882 -lIK220Dll64 -lE1735A -lE1735ACore -o bin\moore_test
.\bin\moore_test.exe
*/