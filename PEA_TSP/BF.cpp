#include "BF.h"

BF::BF(Graph graph) {
    this->nodesNumber = graph.getNodesNumber();
    this->matrix = graph.getMatrix();
}

TSP_Result BF::findCheapestHamiltonianCircle() {

    TSP_Result result;

    std::vector<int> bestPath;
    std::vector<int> path;
    path.push_back(0); // Starting from vertex 0
    std::vector<bool> visited(nodesNumber, false); // To track visited vertices
    visited[0] = true; // Mark the starting vertex as visited
    int minPathCost = INT_MAX; // Initialize minPathCost to infinity

    this->req_findCheapestHamiltonianCircle(path, visited, 0, 0, minPathCost, bestPath); // Call the recursive function

    result.bestPath = bestPath;
    result.minPathCost = minPathCost; 

    return result;
}

void BF::req_findCheapestHamiltonianCircle(std::vector<int>&path, std::vector<bool>visited, int current, int currentCost, int& minCost, std::vector<int>& bestPath ) {

    if (path.size() == nodesNumber) {
        // Check if there is an edge to complete the cycle
        if (matrix[current][path[0]] ) {
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
        if (!visited[next] && matrix[current][next] > 0) { // If not visited and there is an edge
            int newCost = currentCost + matrix[current][next];
            // Prune paths that exceed the current minimum cost
            if (newCost >= minCost) {
                continue; // Skip this path
            }

            // Choose the next vertex
            visited[next] = true; // Mark as visited
            path.push_back(next); // Add to the current path

            // Recurse with the next vertex
            req_findCheapestHamiltonianCircle(path, visited, next, newCost, minCost, bestPath);

            // Backtrack
            path.pop_back(); // Remove from current path
            visited[next] = false; // Mark as unvisited
        }
    }
}
