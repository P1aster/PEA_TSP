#include "BF.h"

BF::BF(Graph graph) {
    this->nodesNumber = graph.getNodesNumber();
    this->matrix = graph.getMatrix();
    this->minPathCost = INT_MAX;
}

TSP_Result BF::findCheapestHamiltonianCircle(int start_node) {

    if (start_node > nodesNumber-1 || start_node < 0) {
		throw std::invalid_argument("Invalid start node");
    }

    this->minPathCost = INT_MAX;
    TSP_Result result;

    std::vector<int> path;
    path.push_back(start_node);
    
    std::vector<bool> visited(nodesNumber, false);
    visited[start_node] = true;

    this->req_findCheapestHamiltonianCircle(path, visited, start_node, 0);

    result.bestPath = bestPath;
    result.minPathCost = minPathCost; 

    return result;
}

void BF::req_findCheapestHamiltonianCircle(std::vector<int>path, std::vector<bool>visited, int current, int currentCost) {

    if (path.size() == nodesNumber) {
        if (matrix[path.back()][path[0]] != -1) {
            int totalCost = currentCost + matrix[path.back()][path[0]];
            if (totalCost < minPathCost) {
                minPathCost = totalCost;
                bestPath = path;
                bestPath.push_back(path[0]);
            }
        }
        return;
    }
    for (int next = 0; next < nodesNumber; next++) {
        if (!visited[next] && matrix[current][next] != -1) {
            int newCost = currentCost + matrix[current][next];
            if (newCost >= minPathCost) {
                continue; 
            }
            visited[next] = true;
            path.push_back(next);
            req_findCheapestHamiltonianCircle(path, visited, next, newCost);
            path.pop_back(); 
            visited[next] = false; 
        }
    }
}
