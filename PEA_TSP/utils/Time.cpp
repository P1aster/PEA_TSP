#include "Time.h"

void Time::start() {
    startTime = std::chrono::high_resolution_clock::now();
}

void Time::stop() {
    stopTime = std::chrono::high_resolution_clock::now();
}

float Time::getElapsedTime() {
    // A floating point milliseconds definition type
    using FP_time =
        std::chrono::duration<float, std::chrono::milliseconds::period>;

    // Check if the rep is a floating point
    static_assert(std::chrono::treat_as_floating_point<FP_time::rep>::value, "Rep required to be floating point");

    // Return the elapsed time in milliseconds
    return FP_time(stopTime - startTime).count();
}



