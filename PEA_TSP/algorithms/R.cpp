#include "R.h"
#include <random>
#include <numeric>

R::R(Graph graph) {
    this->nodesNumber = graph.getNodesNumber();
    this->matrix = graph.getMatrix();
    this->minPathCost = INT_MAX;
}

TSP_Result R::findBestRandomHamiltonianCircle(int permutations) {
    std::random_device rd;
    std::mt19937 g(rd());

    TSP_Result result;
    minPathCost = INT_MAX;

    std::vector<int> nodes(nodesNumber);
    std::iota(nodes.begin(), nodes.end(), 0); // Fill nodes with 0, 1, ..., nodesNumber-1

    for (int i = 0; i < permutations; ++i) { // Try [n] random permutations
        std::shuffle(nodes.begin(), nodes.end(), g);

        int currentCost = 0;
        bool validCycle = true;

        for (int j = 0; j < nodesNumber; ++j) {
            int from = nodes[j];
            int to = nodes[(j + 1) % nodesNumber];
            if (matrix[from][to] <= 0) {
                validCycle = false;
                break;
            }
            currentCost += matrix[from][to];
        }

        if (validCycle && currentCost < result.minPathCost) {
            result.minPathCost = currentCost;
            result.bestPath = nodes;
            result.bestPath.push_back(nodes[0]);
        }
    }

    result.bestPath;
	result.minPathCost = minPathCost;

    return result;
}

TSP_Result R::findBestRandomHamiltonianCircle(std::optional<int> permutations, int knownMinPathCost) {
    std::random_device rd;
    std::mt19937 g(rd());

    TSP_Result result;
    minPathCost = INT_MAX;

    std::vector<int> nodes(nodesNumber);
    std::iota(nodes.begin(), nodes.end(), 0); // Fill nodes with 0, 1, ..., nodesNumber-1

    int maxPermutations = permutations.value_or(std::numeric_limits<int>::max());
    int totalPermutations = std::min(maxPermutations, static_cast<int>(std::tgamma(nodesNumber + 1))); // n!

    for (int i = 0; i < totalPermutations; ++i) { // Try [n] random permutations
        std::shuffle(nodes.begin(), nodes.end(), g);

        int currentCost = 0;
        bool validCycle = true;

        for (int j = 0; j < nodesNumber; ++j) {
            int from = nodes[j];
            int to = nodes[(j + 1) % nodesNumber];
            if (matrix[from][to] <= 0) {
                validCycle = false;
                break;
            }
            currentCost += matrix[from][to];
        }

        if (validCycle && currentCost < result.minPathCost) {
            result.minPathCost = currentCost;
            result.bestPath = nodes;
            result.bestPath.push_back(nodes[0]);
        }

        if (result.minPathCost <= knownMinPathCost) {
            break;
        }
    }

    result.minPathCost = minPathCost;

    return result;
}
