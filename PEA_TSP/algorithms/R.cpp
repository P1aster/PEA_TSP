#include "R.h"
#include <random>

R::R(Graph graph) {
    this->nodesNumber = graph.getNodesNumber();
    this->matrix = graph.getMatrix();
}

TSP_Result R::RandomCircle() {

    TSP_Result result;

	std::vector<int> path;
    std::vector<int> bestPath;
    int minPathCost = INT_MAX;

    findCycle();

	result.bestPath = bestPath;
	result.minPathCost = minPathCost;

	return result;
}


int R::getRandomInt(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(min, max);
	return distr(gen);
}

int R::getNextRandomVertex(std::vector<bool>visited, int current) {


    std::vector<int> candidates;
    for (int i = 0; i < nodesNumber; ++i) {
        if (!visited[i] && matrix[current][i] > 0) {
            candidates.push_back(i);
        }
    }
    if (candidates.empty()) {
        return -1;
    }
    int randomIndex = getRandomInt(0, candidates.size()-1);
    return candidates[randomIndex];
   ;
}

void R::findCycle(std::vector<int>&path, int& minCost) {
    
    bool stopper = true;
    int currentCost = 0;
    std::vector<bool> visited(nodesNumber, false);
    int random = getRandomInt(0, nodesNumber - 1);
    visited[random] = true;
    int current = random;
    int next;

    do {
        if (path.size() == nodesNumber) {
            if (matrix[path.back()][path[0]] > 0) {
                int totalCost = currentCost + matrix[current][path[0]];
                path.push_back(path[0]);
            }
            stopper = false;
        }
        else {
			next = getNextRandomVertex(visited, current);
			if (current == -1) {
				stopper = false;
			} else {
				currentCost += matrix[current][next];
				visited[next] = true;
				path.push_back(next);
				current = next;
			}
        }

        if (currentCost > minCost) {
            stopper = false;
        }

    } while (stopper);
}