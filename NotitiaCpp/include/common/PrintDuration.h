#pragma once

#include <iostream>
#include <chrono>

class PrintDuration {
private:
    std::chrono::high_resolution_clock::time_point initial_time;

public:
    PrintDuration() {
        initial_time = std::chrono::high_resolution_clock::now();
    }

    ~PrintDuration() {
        auto ending_time = std::chrono::high_resolution_clock::now();
        auto ms1 = std::chrono::duration_cast<std::chrono::milliseconds>(ending_time - initial_time);
        std::cout << ms1 << "\n";
    }
};
