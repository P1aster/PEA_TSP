#pragma once

#include "vector"
#include "../structures/Graph.h"
#include "../structures/TSP_Result.h"

// Brute Force
class BF {
private:
    int nodesNumber;
    int** matrix;
    std::vector<int> bestPath;
    int minPathCost;
    void req_findCheapestHamiltonianCircle(std::vector<int> path, std::vector<bool> visited, int current, int currentCost);

public:
    BF(Graph graph);
    TSP_Result findCheapestHamiltonianCircle(int start_node);
};
