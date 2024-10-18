#pragma once

#include <limits>
#include <algorithm>
#include "../structures/Graph.h"
#include "../structures/TSP_Result.h"

class BB {
private:
    int nodesNumber;
    int** matrix;

    std::vector<int> bestPath;
    int minPathCost;

    int lowerBound(std::vector<bool>& visited);

    void req_findCheapestHamiltonianCircle(std::vector<int>& currentPath, std::vector<bool>& visited, int currentCost);

public:
    BB(Graph graph);
    TSP_Result findCheapestHamiltonianCircle();
};

