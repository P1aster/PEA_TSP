#pragma once
#include "structures/Graph.h"
#include "structures/TSP_Result.h"
#include <vector>

class R {
private:
    int nodesNumber;
    int** matrix;
    std::vector<int> bestPath;
    int minPathCost = INT_MAX;
	int knownMinPathCost;
    int getRandomInt();
	int getRandomVertex(std::vector<bool> visited);
	void req_findRandomHamiltonianCircle(std::vector<int> path, std::vector<bool> visited, int current, int currentCost);

public:
	R(Graph graph, int knownMinPathCost);
    TSP_Result findRandomHamiltonianCircle();

};

