#pragma once

#include <limits>
#include <algorithm>
#include "../structures/Graph.h"
#include "../structures/TSP_Result.h"
#include <optional>


class BB {
private:
    int nodesNumber;
    int** matrix;

	std::vector<int> min_edge_cost;
    std::vector<int> bestPath;
    std::vector<int> currPath;
	std::vector<bool> visited;
    int currPathCost = 0;
    int minPathCost = INT_MAX;

    void findCheapest();
    int lowerBound(std::vector<int>& path, int current_node);
    int lowerBound(std::vector<bool>& visited, int current_node);
    void req_findCheapestHamiltonianCircle(std::vector<int>& currentPath, std::vector<bool>& visited, int current, int currentCost);
public:
    BB(Graph graph);
    TSP_Result findCheapestHamiltonianCircle_DFS(int start_node, std::optional<int> upper_limit = std::nullopt);
    TSP_Result findCheapestHamiltonianCircle_LC(int start_node, std::optional<int>upper_limit = std::nullopt);
    TSP_Result findCheapestHamiltonianCircle_BFS(int start_node, std::optional<int> upper_limit = std::nullopt);

};






