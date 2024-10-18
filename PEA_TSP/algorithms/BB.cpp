#include "BB.h"

BB::BB(Graph graph) {
    this->nodesNumber = graph.getNodesNumber();
    this->matrix = graph.getMatrix();
	this->minPathCost = INT_MAX;
}

TSP_Result BB::findCheapestHamiltonianCircle() {
    TSP_Result result;
    std::vector<int> path;
    path.push_back(0);
    std::vector<bool> visited(nodesNumber, false);
    visited[0] = true;

    this->req_findCheapestHamiltonianCircle(path, visited, 0);

    result.bestPath = bestPath;
    result.minPathCost = minPathCost;

    return result;
}

void BB::req_findCheapestHamiltonianCircle(std::vector<int>& path, std::vector<bool>& visited, int currentCost) {
    if (path.size() == nodesNumber) {
        if (matrix[path.back()][path[0]] > 0) {
            int totalCost = currentCost + matrix[path.back()][path[0]];
            if (totalCost < minPathCost) {
                minPathCost = totalCost;
                bestPath = path;
                bestPath.push_back(path[0]);
            }
        }
        return;
    }

    for (int next = 0; next < nodesNumber; ++next) {
        if (!visited[next] && matrix[path.back()][next] > 0) {
            int newCost = currentCost + matrix[path.back()][next];
            int bound = newCost + lowerBound(visited);
            if (bound < minPathCost) {
                visited[next] = true;
                path.push_back(next);
                req_findCheapestHamiltonianCircle(path, visited, newCost);
                path.pop_back();
                visited[next] = false;
            }
        }
    }
}



int BB::lowerBound(std::vector<bool>& visited) {
    int bound = 0;
    for (int i = 0; i < nodesNumber; ++i) {
        if (!visited[i]) {
            int minEdge = INT_MAX;
            for (int j = 0; j < nodesNumber; ++j) {
                if (i != j && !visited[j]) {
                    minEdge = std::min(minEdge, matrix[i][j]);
                }
            }
            bound += (minEdge != INT_MAX) ? minEdge : 0;
        }
    }
    return bound;
}

