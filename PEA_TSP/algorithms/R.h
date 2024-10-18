#pragma once

#include "../structures/Graph.h"
#include "../structures/TSP_Result.h"

class R {
private:
    int nodesNumber;
    int** matrix;

    int getNextRandomVertex(std::vector<bool>visited, int current);
    int getRandomInt(int min, int max);
    void findCycle(int current, int currentCost, int& minCost, std::vector<int>& bestPath);

public:
    R(Graph graph);
    TSP_Result RandomCircle();

};

