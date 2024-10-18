#pragma once

#include "vector"
#include "../structures/Graph.h"
#include "../structures/TSP_Result.h"



// Brute Force
class BF {
private:
    int nodesNumber;
    int** matrix;
    void req_findCheapestHamiltonianCircle(std::vector<int>& path, std::vector<bool> visited, int current, int currentCost, int& minCost, std::vector<int>& bestPath);

public:
    BF(Graph graph);
    TSP_Result findCheapestHamiltonianCircle();
};

