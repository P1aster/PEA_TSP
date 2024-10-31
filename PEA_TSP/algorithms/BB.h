#pragma once

#include <limits>
#include <algorithm>
#include "../structures/Graph.h"
#include "../structures/TSP_Result.h"


class BB {
private:
    int nodesNumber;
    int** matrix;

	std::vector<int> min_edge_cost;
    std::vector<int> bestPath;
    int minPathCost;

    void findCheapest();
    int lowerBound(std::vector<int>& path, int current_node);
    int lowerBound(std::vector<bool>& visited, int current_node);

    void req_findCheapestHamiltonianCircle(std::vector<int>& currentPath, std::vector<bool>& visited, int current, int currentCost);

public:
    BB(Graph graph);
    TSP_Result findCheapestHamiltonianCircle_DFS(int start_node);
    TSP_Result findCheapestHamiltonianCircle_BFS(int start_node);
};






