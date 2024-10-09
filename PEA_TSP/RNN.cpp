#include "RNN.h"


RNN::RNN(Graph graph) {
    this->nodesNumber = graph.getNodesNumber();
    this->matrix = graph.getMatrix();
}

TSP_Result RNN::findRepeatedNearestNaighbour() {
    TSP_Result result;

    std::vector<int> totalBestPath;
    int totalMinPathCost = INT_MAX; // Initialize minPathCost to infinity

    std::vector<int> bestPath;
    int minPathCost = INT_MAX; // Initialize minPathCost to infinity

    std::vector<int> path;
    std::vector<bool> visited(nodesNumber, false); // To track visited vertices

    for (int i = 0; i < nodesNumber; i++) {
        this->findNearestNaighbour(bestPath, path, i, visited, minPathCost);
        if (minPathCost < totalMinPathCost) {
            totalMinPathCost = minPathCost;
            totalBestPath = bestPath;
        }
        minPathCost = INT_MAX;
        bestPath.clear();
        path.clear();
        visited = std::vector<bool>(nodesNumber, false);
    }

    result.bestPath = totalBestPath;
    result.minPathCost = totalMinPathCost;

    return result;
}
TSP_Result RNN::findNearestNaighbour() {
    TSP_Result result;

    std::vector<int> bestPath;
    std::vector<int> path;
    std::vector<bool> visited(nodesNumber, false); // To track visited vertices
    int minPathCost = INT_MAX; // Initialize minPathCost to infinity

    this->req_findNearestNaighbour(path, visited, 0, 0, minPathCost, bestPath);

    result.bestPath = bestPath;
    result.minPathCost = minPathCost;

    return result;
}

void RNN::findNearestNaighbour(std::vector<int>& bestPath, std::vector<int>& path, int current, std::vector<bool> visited, int& minPathCost) {
    visited[current] = true; // Mark the current vertex as visited
    path.push_back(current); // Add the current vertex to the path

    this->req_findNearestNaighbour(path, visited, current, 0, minPathCost, bestPath);
}

void RNN::req_findNearestNaighbour(std::vector<int>& path, std::vector<bool>visited, int current, int currentCost, int& minCost, std::vector<int>& bestPath) {
    int bestNextCost = INT_MAX;
    int bestNextNode = -1;
    
    if (path.size() == nodesNumber) {
        // Check if there is an edge to complete the cycle
        if (matrix[current][path[0]]) {
            int totalCost = currentCost + matrix[current][path[0]];
            if (totalCost < minCost) {
                minCost = totalCost;
                bestPath = path;
                bestPath.push_back(path[0]);
            }
        }
        return;
    }

    // Try to visit the nearest unvisited vertex
    for (int next = 0; next < nodesNumber; next++) {
        if (matrix[current][next] > 0 && matrix[current][next] < bestNextCost && !visited[next]) {
            bestNextCost = matrix[current][next];
            bestNextNode = next;
        }
    }

    // Choose the next vertex
    visited[bestNextNode] = true; // Mark as visited
    path.push_back(bestNextNode); // Add to the current path

    // Recurse with the next vertex
    req_findNearestNaighbour(path, visited, bestNextNode, currentCost + bestNextCost, minCost, bestPath);
    
    return;
}


