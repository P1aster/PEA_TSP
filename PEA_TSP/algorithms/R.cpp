#include "R.h"


R::R(Graph graph) {
    this->nodesNumber = graph.getNodesNumber();
    this->matrix = graph.getMatrix();
	this->knownMinPathCost = 0;
    this->minPathCost = INT_MAX;
}

int R::getRandomVertex(std::vector<bool> visited) {
    std::vector<int> unvisited;

    for (int i = 0; i < nodesNumber; ++i) {
        if (!visited[i]) {
            unvisited.push_back(i);
        }
    }

    if (!unvisited.empty()) {
        int randIndex = rand() % unvisited.size();
        return unvisited[randIndex];
    }
    return -1;

}

TSP_Result R::findRandomHamiltonianCircle() {

	srand(time(nullptr));

    this->minPathCost = INT_MAX;
	TSP_Result result;
	std::vector<int> totalBestPath;
	int totalMinPathCost = INT_MAX;
	std::vector<int> path;
	std::vector<bool> visited(nodesNumber, false);

	int start_node = rand() % nodesNumber;

	path.push_back(start_node);
	visited[start_node] = true;

	this->req_findRandomHamiltonianCircle(path, visited, start_node, 0);

	result.bestPath = bestPath;
	result.minPathCost = minPathCost;

	return result;
}

void R::req_findRandomHamiltonianCircle(std::vector<int> path, std::vector<bool> visited, int current, int currentCost) {
    int newCost = 0;

    
    if (path.size() == nodesNumber) {
        if (matrix[path.back()][path[0]] > 0) {
            int totalCost = currentCost + matrix[current][path[0]];
            if (totalCost < minPathCost) {
                minPathCost = totalCost;
                bestPath = path;
                bestPath.push_back(path[0]);
            }
        }
        return;
    }

    for (int attempt = 0; attempt < nodesNumber - path.size(); ++attempt) {
        int next = getRandomVertex(visited);
        if (next == -1 || matrix[current][next] <= 0) {
            continue; 
        }

        visited[next] = true;
        path.push_back(next);

        newCost = currentCost + matrix[current][next];

        req_findRandomHamiltonianCircle(path, visited, next, newCost);

        visited[next] = false;
        path.pop_back();
    }
}



