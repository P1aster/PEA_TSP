#pragma once
#include <chrono>


class Time {
private:
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point stopTime;
\
public: 
    void start();
    void stop();
    float getElapsedTime();
    float getElapsedTimeNow();

};

