#pragma once

#include <vector>
#include "../structures/Graph.h"
#include "../structures/TSP_Result.h"


class R {
private:
    int nodesNumber;
    int** matrix;
    std::vector<int> bestPath;
    int minPathCost;
	int knownMinPathCost;
	int getRandomVertex(std::vector<bool> visited);
	void req_findRandomHamiltonianCircle(std::vector<int> path, std::vector<bool> visited, int current, int currentCost);

public:
	R(Graph graph);
    TSP_Result findRandomHamiltonianCircle();

};

