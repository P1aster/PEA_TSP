#pragma once
#include "../structures/Graph.h"
#include "../structures/TSP_Result.h"

// Repeated Nearest Neighbor
class RNN {
private:
    int nodesNumber;
    int** matrix;
    std::vector<int> bestPath;
    int minPathCost = INT_MAX;
    void req_findNearestNaighbour(std::vector<int>path, std::vector<bool>visited, int current, int currentCost);

public:
    RNN(Graph graph);
    TSP_Result findRepeatedNearestNaighbour();
    TSP_Result findNearestNaighbour(int start_node);
};
