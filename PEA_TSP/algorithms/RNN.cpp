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
    std::vector<bool> visited;

    for (int i = 0; i < nodesNumber; i++) {

        this->minPathCost = INT_MAX;
        this->bestPath.clear();
        path.clear();
        path.push_back(i); 

        visited = std::vector<bool>(nodesNumber, false);
        visited[i] = true; 


        this->req_findNearestNaighbour(path, visited, i, 0);

        if (minPathCost < totalMinPathCost) {
            totalMinPathCost = minPathCost;
            totalBestPath = bestPath;
        }      
        
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


TSP_Result RNN::findBestNearestNeighbour(int start_node) {
    if (start_node > nodesNumber - 1) {
        throw std::invalid_argument("Invalid start node");
    }

    this->minPathCost = INT_MAX;
    TSP_Result result;

    std::vector<int> path;
    path.push_back(start_node);
    
    std::vector<bool> visited(nodesNumber, false);
    visited[start_node] = true;

    this->req_findBestNearestNaighbour(path, visited, start_node, 0);

    result.bestPath = bestPath;
    result.minPathCost = minPathCost;

    return result;
}

TSP_Result RNN::findBestRepeatedNearestNeighbour() {

    TSP_Result result;

    std::vector<int> totalBestPath;
    int totalMinPathCost = INT_MAX;

    std::vector<int> path;
    std::vector<bool> visited;

    for (int i = 0; i < nodesNumber; i++) {

        this->minPathCost = INT_MAX;
        this->bestPath.clear();
        path.clear();
        path.push_back(i);

        visited = std::vector<bool>(nodesNumber, false);
        visited[i] = true;

        this->req_findBestNearestNaighbour(path, visited, i, 0);
        if (minPathCost < totalMinPathCost) {
            totalMinPathCost = minPathCost;
            totalBestPath = bestPath;
        }
    }

    result.bestPath = totalBestPath;
    result.minPathCost = totalMinPathCost;

    return result;
}


void RNN::req_findNearestNaighbour(std::vector<int> path, std::vector<bool>visited, int current, int currentCost) {
    if (path.size() == nodesNumber) {
        if (matrix[current][path[0]] != -1) {
            int totalCost = currentCost + matrix[current][path[0]];
            if (totalCost < minPathCost) {
                minPathCost = totalCost;
                bestPath = path;
                bestPath.push_back(path[0]);
            }
        }
        return;
    }

    int bestNextCost = INT_MAX;
    int bestNextNode = -1;

    for (int next = 0; next < nodesNumber; next++) {
        int nextCost = matrix[current][next];

        if (nextCost != -1 && nextCost < bestNextCost && !visited[next]) {
            bestNextCost = nextCost;
            bestNextNode = next;
        }
    }

    visited[bestNextNode] = true;
    path.push_back(bestNextNode); 

    req_findNearestNaighbour(path, visited, bestNextNode, currentCost + bestNextCost);
    
    //return;
}

void RNN::req_findBestNearestNaighbour(std::vector<int> path, std::vector<bool>visited, int current, int currentCost) {

    if (path.size() == nodesNumber) {
        if (matrix[current][path[0]] != -1) {
            int totalCost = currentCost + matrix[current][path[0]];
            if (totalCost < minPathCost) {
                minPathCost = totalCost;
                bestPath = path;
                bestPath.push_back(path[0]);
            }
        }
        return;
    }

    int bestNextCost = INT_MAX;
    std::vector<int> bestNextNodes;

    for (int next = 0; next < nodesNumber; next++) {
        if (!visited[next] && matrix[current][next] != -1) {
			int nextCost = matrix[current][next];
            if (nextCost < bestNextCost) {
                bestNextCost = nextCost;
                bestNextNodes.clear();
                bestNextNodes.push_back(next);
            }
            else if (nextCost == bestNextCost) {
                bestNextNodes.push_back(next);
            }
        }
    }

    for (int next : bestNextNodes) {
        visited[next] = true;
        path.push_back(next);
        req_findNearestNaighbour(path, visited, next, currentCost + bestNextCost);
        path.pop_back();
        visited[next] = false;
    }
}


