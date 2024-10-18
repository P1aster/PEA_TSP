#pragma once
#include "../structures/Graph.h"
#include "../structures/TSP_Result.h"

// Repeated Nearest Neighbor
class RNN {
private:
    int nodesNumber;
    int** matrix;

    void findNearestNaighbour(std::vector<int>& bestPath, std::vector<int>& path, int current, std::vector<bool> visited, int& minPathCost);
    void req_findNearestNaighbour(std::vector<int>& path, std::vector<bool>visited, int current, int currentCost, int& minCost, std::vector<int>& bestPath);

public:
    RNN(Graph graph);
    TSP_Result findRepeatedNearestNaighbour();
    TSP_Result findNearestNaighbour();
};
