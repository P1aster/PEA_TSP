#include "RNN.h"


RNN::RNN(Graph graph) {
    this->nodesNumber = graph.getNodesNumber();
    this->matrix = graph.getMatrix();
}

TSP_Result RNN::findRepeatedNearestNaighbour() {
    TSP_Result result;

    std::vector<int> totalBestPath;
    int totalMinPathCost = INT_MAX; 

    std::vector<int> bestPath;
    int minPathCost = INT_MAX; 

    std::vector<int> path;
    std::vector<bool> visited(nodesNumber, false);


    for (int i = 0; i < nodesNumber; i++) {

        path.push_back(i); 
        visited[i] = true; 
        minPathCost = INT_MAX;

        this->req_findNearestNaighbour(path, visited, i, 0, minPathCost, bestPath);
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
    std::vector<bool> visited(nodesNumber, false);
    path.push_back(0);
    visited[0] = true;

    int minPathCost = INT_MAX; 

    this->req_findNearestNaighbour(path, visited, 0, 0, minPathCost, bestPath);

    result.bestPath = bestPath;
    result.minPathCost = minPathCost;

    return result;
}

void RNN::findNearestNaighbour(std::vector<int>& bestPath, std::vector<int>& path, int current, std::vector<bool> visited, int& minPathCost) {
    visited[current] = true; 
    path.push_back(current); 

    this->req_findNearestNaighbour(path, visited, current, 0, minPathCost, bestPath);
}

void RNN::req_findNearestNaighbour(std::vector<int>& path, std::vector<bool>visited, int current, int currentCost, int& minCost, std::vector<int>& bestPath) {
    int bestNextCost = INT_MAX;
    int bestNextNode = -1;
    
    if (path.size() == nodesNumber) {
        if (matrix[current][path[0]] > 0) {
            int totalCost = currentCost + matrix[current][path[0]];
            if (totalCost < minCost) {
                minCost = totalCost;
                bestPath = path;
                bestPath.push_back(path[0]);
            }
        }
        return;
    }

    for (int next = 0; next < nodesNumber; next++) {
        if (matrix[current][next] > 0 && matrix[current][next] < bestNextCost && !visited[next]) {
            bestNextCost = matrix[current][next];
            bestNextNode = next;
        }
    }

    visited[bestNextNode] = true;
    path.push_back(bestNextNode); 

    req_findNearestNaighbour(path, visited, bestNextNode, currentCost + bestNextCost, minCost, bestPath);
    
    return;
}


