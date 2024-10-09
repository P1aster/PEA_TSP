#pragma once

#include <vector>
#include "Graph.h"


// Nearest Neighbor
class NN {
private:
    int nodesNumber;
    int** matrix;
    void req_findCheapestHamiltonianCircle(std::vector<int>& path, std::vector<bool>visited, int current, int currentCost, int& minCost, std::vector<int>& bestPath);

public:
    NN(Graph graph);
    HamiltonianResult findCheapestHamiltonianCircle();
};

