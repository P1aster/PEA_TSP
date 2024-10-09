#include "Time.h"

void Time::start() {
    startTime = std::chrono::high_resolution_clock::now();
}

void Time::stop() {
    stopTime = std::chrono::high_resolution_clock::now();
}

long long Time::getElapsedTime() {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(stopTime - startTime).count();
}


