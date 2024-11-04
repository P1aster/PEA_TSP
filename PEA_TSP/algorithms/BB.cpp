#include "BB.h"
#include <queue>
#include "../structures/Node.h"
#include <stack>



BB::BB(Graph graph) {
    this->nodesNumber = graph.getNodesNumber();
    this->matrix = graph.getMatrix();
	this->minPathCost = INT_MAX;
}
TSP_Result BB::findCheapestHamiltonianCircle_DFS(int start_node) {

    if (start_node > nodesNumber - 1 || start_node < 0) {
        throw std::invalid_argument("Invalid start node");
    }

    this->minPathCost = INT_MAX;
    TSP_Result result;
    std::vector<int> path;
    path.push_back(start_node);
    std::vector<bool> visited(nodesNumber, false);
    visited[start_node] = true;

    this->findCheapest();
    this->req_findCheapestHamiltonianCircle(path, visited, start_node, 0);

    result.bestPath = bestPath;
    result.minPathCost = minPathCost;

    return result;
}

TSP_Result BB::findCheapestHamiltonianCircle_CL(int start_node) {

    if (start_node > nodesNumber - 1 || start_node < 0) {
        throw std::invalid_argument("Invalid start node");
    }

    this->minPathCost = INT_MAX;
    TSP_Result result;
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq; //smallest elements will apear on front 

    std::vector<int> pom_path;
    int pom_cost;
    int pom_bound;

    this->findCheapest();


    // Initialize with the start node
    std::vector<int> initial_path = { start_node };
    int initial_bound = lowerBound(initial_path, start_node);
    pq.push(Node(start_node, initial_path, 0, initial_bound, 0));

    while (!pq.empty()) {
        Node current = pq.top();
        pq.pop();

        //Check for Hamiltonian Cyrcle
        if (current.level == nodesNumber - 1) {
            int return_cost = matrix[current.current_vertex][start_node];
            if (return_cost > 0) {
                int final_cost = current.cost + return_cost;
                if (final_cost < minPathCost) {
                    minPathCost = final_cost;
                    bestPath = current.path;
                    bestPath.push_back(start_node);
                }
            }
            continue;
        }

        // Explore the neighbors (next level)
        for (int next_vertex = 0; next_vertex < nodesNumber; ++next_vertex) {
			// Ignore vertices that are already in the path or have no edge, find will return current.path.end() if the vertex is not in the path
            if (matrix[current.current_vertex][next_vertex] <= 0 || std::find(current.path.begin(), current.path.end(), next_vertex) != current.path.end()) {
                continue;
            }

            // Calculate new cost and lower bound
            pom_cost = current.cost + matrix[current.current_vertex][next_vertex];
            pom_path = current.path;
            pom_path.push_back(next_vertex);

			// Calculate the lower bound
            pom_bound = pom_cost + lowerBound(pom_path, next_vertex);
			// If the lower bound is less than the current minimum path cost, add the node to the queue
            if (pom_bound < minPathCost) {
                pq.push(Node(next_vertex, pom_path, pom_cost, pom_bound, current.level + 1));
            }
        }
    }


	result.bestPath = bestPath;
	result.minPathCost = minPathCost;

    return result;


}


TSP_Result BB::findCheapestHamiltonianCircle_FILO(int start_node) {
    if (start_node > nodesNumber - 1 || start_node < 0) {
        throw std::invalid_argument("Invalid start node");
    }

    this->minPathCost = INT_MAX;
    TSP_Result result;
    std::stack<Node> s;

    std::vector<int> pom_path;
    int pom_cost;
    int pom_bound;

    this->findCheapest();

    // Initialize with the start node
    std::vector<int> initial_path = { start_node };
    int initial_bound = lowerBound(initial_path, start_node);
    s.push(Node(start_node, initial_path, 0, initial_bound, 0));

    while (!s.empty()) {
        Node current = s.top();
        s.pop();

        // Check for Hamiltonian Cycle
        if (current.level == nodesNumber - 1) {
            int return_cost = matrix[current.current_vertex][start_node];
            if (return_cost > 0) {
                int final_cost = current.cost + return_cost;
                if (final_cost < minPathCost) {
                    minPathCost = final_cost;
                    bestPath = current.path;
                    bestPath.push_back(start_node);
                }
            }
            continue;
        }

        // Explore the neighbors (next level)
        for (int next_vertex = 0; next_vertex < nodesNumber; ++next_vertex) {
            // Ignore vertices that are already in the path or have no edge
            if (matrix[current.current_vertex][next_vertex] <= 0 || std::find(current.path.begin(), current.path.end(), next_vertex) != current.path.end()) {
                continue;
            }

            // Calculate new cost and lower bound
            pom_cost = current.cost + matrix[current.current_vertex][next_vertex];
            pom_path = current.path;
            pom_path.push_back(next_vertex);

            // Calculate the lower bound
            pom_bound = pom_cost + lowerBound(pom_path, next_vertex);
            // If the lower bound is less than the current minimum path cost, add the node to the stack
            if (pom_bound < minPathCost) {
                s.push(Node(next_vertex, pom_path, pom_cost, pom_bound, current.level + 1));
            }
        }
    }

    result.bestPath = bestPath;
    result.minPathCost = minPathCost;

    return result;
}
void BB::req_findCheapestHamiltonianCircle(std::vector<int>& path, std::vector<bool>& visited, int current, int currentCost) {

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
        if (!visited[next] && matrix[current][next] > 0) {
            int newCost = currentCost + matrix[current][next];
            visited[next] = true;
            path.push_back(next);
            int bound = newCost + lowerBound(visited, next);
            if (bound < minPathCost) {
                req_findCheapestHamiltonianCircle(path, visited, next, newCost);
            }
            path.pop_back();
            visited[next] = false;
        }
    }
}

int BB::lowerBound(std::vector<bool>& visited, int current_node) {
    int currentBound = 0;
    for (int i = 0; i < nodesNumber; i++) {
        if (!visited[i] && i != current_node) {
            currentBound += min_edge_cost[i];
        }
    }
    return currentBound;
}

int BB::lowerBound(std::vector<int>& path, int current_node) {
    int lower_bound = 0;
    std::vector<bool> visited = std::vector<bool>(nodesNumber, false);

    // Mark nodes in the path as visited
    for (int node : path) visited[node] = true;

    // Add the minimum edge cost for each unvisited city
    for (int i = 0; i < nodesNumber; i++) {
        if (!visited[i]) {
            lower_bound += min_edge_cost[i];
        }
    }
    return lower_bound;
};


void BB::findCheapest() {

    min_edge_cost = std::vector<int>(nodesNumber, INT_MAX);

    for (int i = 0; i < nodesNumber; i++) {      
        for (int j = 0; j < nodesNumber; j++) {  
            if (i != j && min_edge_cost[i] > matrix[i][j]) {
                min_edge_cost[i] = matrix[i][j];
            }
        }
    }
}

