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
    this->minPathCost = INT_MAX;

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
