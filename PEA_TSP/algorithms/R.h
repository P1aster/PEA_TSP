#pragma once

#include <vector>
#include "../structures/Graph.h"
#include "../structures/TSP_Result.h"
#include <optional>


class R {
private:
    int nodesNumber;
    int** matrix;
    std::vector<int> bestPath;
    int minPathCost;
	int knownMinPathCost;

public:
	R(Graph graph);
    TSP_Result findBestRandomHamiltonianCircle();
    TSP_Result findBestRandomHamiltonianCircle(int known_minPathCost);

    TSP_Result findBestRandomHamiltonianCircle(std::optional<int> permutations, int knownMinPathCost);



};

