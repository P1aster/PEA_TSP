#pragma once
#include <vector>
#include <optional>
#include "../structures/Graph.h"
#include "../structures/TSP_Result.h"


class R {
private:
    int nodesNumber;
    int** matrix;
    std::vector<int> bestPath;
    int minPathCost;
	int knownMinPathCost;

public:
	R(Graph graph);
    //TSP_Result findBestRandomHamiltonianCircle(int permutations);
    TSP_Result findBestRandomHamiltonianCircle(std::optional<int> permutations = std::nullopt, std::optional<int> knownMinPathCost = std::nullopt, std::optional<int> maxDurationMs = std::nullopt);
};

