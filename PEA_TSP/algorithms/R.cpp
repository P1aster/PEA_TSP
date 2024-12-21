#include "R.h"
#include <random>
#include <numeric>
#include "../utils/Time.h"

R::R(Graph graph) {
    this->nodesNumber = graph.getNodesNumber();
    this->matrix = graph.getMatrix();
    this->minPathCost = INT_MAX;
	this->knownMinPathCost = INT_MAX;
}

TSP_Result R::findBestRandomHamiltonianCircle(std::optional<int> permutations, std::optional<int> knownMinPathCost, std::optional<int> maxDurationMs) {

    Time time;
   
    std::random_device rd;
    std::mt19937 g(rd());

    TSP_Result result;
    minPathCost = INT_MAX;

    std::vector<int> nodes(nodesNumber);

	int pom_currentCost = 0;
	bool pom_validCycle = true;

    for (int i = 0; i < nodesNumber; ++i) {
        nodes[i] = i;
    }

    unsigned int maxPermutations = permutations.value_or(INT_MAX);
    unsigned int totalPermutations = std::min(static_cast<int>(maxPermutations), static_cast<int>(std::tgamma(nodesNumber + 1))); // n! or maxPermutations
    
    time.start();
    for (unsigned int i = 0; i < totalPermutations; ++i) { // Try [n] random permutations
        
		std::shuffle(nodes.begin(), nodes.end(), g); // Random permutation

        pom_currentCost = 0;
        pom_validCycle = true;

        for (int j = 0; j < nodesNumber-1; j++) {
            int from = nodes[j];
            int to = nodes[j + 1];
            if (matrix[from][to] == -1) {
                pom_validCycle = false;
                break;
            }
            pom_currentCost += matrix[from][to];
        }

        if (pom_validCycle && pom_currentCost < minPathCost) {
            minPathCost = pom_currentCost;
            bestPath = nodes;
            bestPath.push_back(nodes[0]);
        }

        if (minPathCost <= knownMinPathCost) {
            break;
        }

        if (maxDurationMs.has_value()) {
            float elapsedTime = time.getElapsedTimeNow();
            if (elapsedTime >= maxDurationMs.value()) {
                break;
            }
        }
    }

    result.minPathCost = minPathCost;
    result.bestPath = bestPath;


    return result;
}

TSP_Result R::findFirstRandomHamiltonianCircle(std::optional<int> knownMinPathCost, std::optional<int> maxDurationMs) {
    Time time;
    std::random_device rd;
    std::mt19937 g(rd());
    TSP_Result result;

    std::vector<int> nodes(nodesNumber);
    for (int i = 0; i < nodesNumber; ++i) {
        nodes[i] = i;
    }

    time.start();
    while (true) {
        std::shuffle(nodes.begin(), nodes.end(), g); // Random permutation

        int currentCost = 0;
        bool validCycle = true;

        for (int j = 0; j < nodesNumber - 1; ++j) {
            int from = nodes[j];
            int to = nodes[j + 1];
            if (matrix[from][to] == -1) {
                validCycle = false;
                break;
            }
            currentCost += matrix[from][to];
        }

        // Add the cost from the last node back to the first to complete the cycle
        if (validCycle && matrix[nodes[nodesNumber - 1]][nodes[0]] != -1) {
            currentCost += matrix[nodes[nodesNumber - 1]][nodes[0]];
        }
        else {
            validCycle = false;
        }

        if (validCycle) {
            result.bestPath = nodes;
            result.bestPath.push_back(nodes[0]); // Complete the cycle
            result.minPathCost = currentCost;
            return result;
        }

        // Check if known minimum path cost is reached
        if (knownMinPathCost.has_value() && currentCost <= knownMinPathCost.value()) {
            break;
        }

        // Check if maximum duration has been exceeded
        if (maxDurationMs.has_value()) {
            float elapsedTime = time.getElapsedTimeNow();
            if (elapsedTime >= maxDurationMs.value()) {
                break;
            }
        }
    }

    // No valid cycle found within constraints
    result.minPathCost = INT_MAX;
    result.bestPath.clear();
    return result;
}
