#include "RNN.h"


RNN::RNN(Graph graph) {
    this->nodesNumber = graph.getNodesNumber();
    this->matrix = graph.getMatrix();
    this->minPathCost = INT_MAX;
}

TSP_Result RNN::findRepeatedNearestNaighbour() {
    TSP_Result result;

    std::vector<int> totalBestPath;
    int totalMinPathCost = INT_MAX; 

    std::vector<int> path;
    std::vector<bool> visited(nodesNumber, false);


    for (int i = 0; i < nodesNumber; i++) {
        path.push_back(i); 
        visited[i] = true; 
        minPathCost = INT_MAX;

        this->req_findNearestNaighbour(path, visited, i, 0);
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
TSP_Result RNN::findNearestNaighbour(int start_node) {

    if (start_node > nodesNumber - 1) {
        throw std::invalid_argument("Invalid start node");
    }

    this->minPathCost = INT_MAX;
    TSP_Result result;
    std::vector<int> path;
    path.push_back(start_node);
    std::vector<bool> visited(nodesNumber, false);
    visited[start_node] = true;

    this->req_findNearestNaighbour(path, visited, start_node, 0);

    result.bestPath = bestPath;
    result.minPathCost = minPathCost;

    return result;
}

void RNN::req_findNearestNaighbour(std::vector<int> path, std::vector<bool>visited, int current, int currentCost) {
    int bestNextCost = INT_MAX;
    int bestNextNode = -1;
    
    if (path.size() == nodesNumber) {
        if (matrix[current][path[0]] > 0) {
            int totalCost = currentCost + matrix[current][path[0]];
            if (totalCost < minPathCost) {
                minPathCost = totalCost;
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

    req_findNearestNaighbour(path, visited, bestNextNode, currentCost + bestNextCost);
    
    return;
}


