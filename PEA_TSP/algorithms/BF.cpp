#include "BF.h"

BF::BF(Graph graph) {
    this->nodesNumber = graph.getNodesNumber();
    this->matrix = graph.getMatrix();
}

TSP_Result BF::findCheapestHamiltonianCircle() {

    TSP_Result result;

    std::vector<int> bestPath;
    std::vector<int> path;
    path.push_back(0);
    std::vector<bool> visited(nodesNumber, false);
    visited[0] = true; 
    int minPathCost = INT_MAX;

    this->req_findCheapestHamiltonianCircle(path, visited, 0, 0, minPathCost, bestPath);

    result.bestPath = bestPath;
    result.minPathCost = minPathCost; 

    return result;
}

void BF::req_findCheapestHamiltonianCircle(std::vector<int>&path, std::vector<bool>visited, int current, int currentCost, int& minCost, std::vector<int>& bestPath ) {

    int newCost = 0;

    if (path.size() == nodesNumber) {
        // Check if there is an edge to complete the cycle
        if (matrix[path.back()][path[0]] > 0) {
            int totalCost = currentCost + matrix[current][path[0]];
            if (totalCost < minCost) {
                minCost = totalCost;
                bestPath = path;
                bestPath.push_back(path[0]);
            }
        }
        return;
    }


    for (int next = 0; next < nodesNumber; ++next) {
        if (!visited[next] && matrix[current][next] > 0) {
            newCost = currentCost + matrix[current][next];
            if (newCost >= minCost) {
                continue; 
            }

            visited[next] = true;
            path.push_back(next);

            req_findCheapestHamiltonianCircle(path, visited, next, newCost, minCost, bestPath);

            path.pop_back(); 
            visited[next] = false; 
        }
    }
}
