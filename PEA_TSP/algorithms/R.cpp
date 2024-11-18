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

//TSP_Result R::findBestRandomHamiltonianCircle(int permutations) {
//
//    std::random_device rd;
//    std::mt19937 g(rd());
//
//    TSP_Result result;
//    minPathCost = INT_MAX;
//
//    std::vector<int> nodes(nodesNumber);
//
//    std::iota(nodes.begin(), nodes.end(), 0); // Fill nodes with 0, 1, ..., nodesNumber-1
//
//    for (int i = 0; i < permutations; ++i) { // Try [n] random permutations
//        std::shuffle(nodes.begin(), nodes.end(), g);
//
//        int currentCost = 0;
//        bool validCycle = true;
//
//        for (int j = 0; j < nodesNumber; ++j) {
//            int from = nodes[j];
//            int to = nodes[(j + 1) % nodesNumber];
//            if (matrix[from][to] <= 0) {
//                validCycle = false;
//                break;
//            }
//            currentCost += matrix[from][to];
//        }
//
//        if (validCycle && currentCost < minPathCost) {
//            minPathCost = currentCost;
//            bestPath = nodes;
//            bestPath.push_back(nodes[0]);
//        }
//    }
//
//    result.bestPath = bestPath;
//	result.minPathCost = minPathCost;
//
//    return result;
//}
TSP_Result R::findBestRandomHamiltonianCircle(std::optional<int> permutations, std::optional<int> knownMinPathCost, std::optional<int> maxDurationMs) {


    Time time;
    time.start();
   
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
    unsigned maxPermutations = permutations.value_or(INT_MAX);
    unsigned int totalPermutations = std::min(static_cast<int>(maxPermutations), static_cast<int>(std::tgamma(nodesNumber + 1))); // n!
    for (int i = 0; i < totalPermutations; ++i) { // Try [n] random permutations
        std::shuffle(nodes.begin(), nodes.end(), g);

        pom_currentCost = 0;
        pom_validCycle = true;

        for (int j = 0; j < nodesNumber; ++j) {
            int from = nodes[j];
            int to = nodes[(j + 1) % nodesNumber];
            if (matrix[from][to] <= 0) {
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
