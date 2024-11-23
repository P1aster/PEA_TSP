#include "BB.h"
#include <queue>
#include "../structures/Node.h"
#include <stack>



BB::BB(Graph graph) {
    this->nodesNumber = graph.getNodesNumber();
    this->matrix = graph.getMatrix();
	this->minPathCost = INT_MAX;
	this->currPathCost = 0;
	this->visited = std::vector<bool>(nodesNumber, false);
}
TSP_Result BB::findCheapestHamiltonianCircle_DFS(int start_node, std::optional<int> upper_limit) {

    if (start_node > nodesNumber - 1 || start_node < 0) {
        throw std::invalid_argument("Invalid start node");
    }

    TSP_Result result;

    minPathCost = upper_limit.value_or(INT_MAX);

    std::vector<int> path;
    path.push_back(start_node);
    
    std::vector<bool> visited(nodesNumber, false);
    visited[start_node] = true;

    findCheapest();
    req_findCheapestHamiltonianCircle(path, visited, start_node, 0);
    result.bestPath = bestPath;
    result.minPathCost = minPathCost;

    return result;
}
TSP_Result BB::findCheapestHamiltonianCircle_LC(int start_node, std::optional<int> upper_limit) {
    if (start_node > nodesNumber - 1 || start_node < 0) {
        throw std::invalid_argument("Invalid start node");
    }
    TSP_Result result;

    struct PiorityQueueNodeState {
        std::vector<int> path;  // Stores the current path
        int cost;               // Cost of the path so far
        int lower_bound;        // Lower bound on the cost to complete this path
        int level;              // Level of the node (depth in tree)
        int node;     // Current vertex in the path

        PiorityQueueNodeState(int node, std::vector<int> path, int cost, int lower_bound, int level) :
            node(node), path(path), cost(cost), lower_bound(lower_bound), level(level) {
        }

        // Comparator for priority queue
        bool operator>(const PiorityQueueNodeState& other) const {
            return lower_bound > other.lower_bound;
        }
    };


    minPathCost = upper_limit.value_or(INT_MAX);
    std::priority_queue<PiorityQueueNodeState, std::vector<PiorityQueueNodeState>, std::greater<PiorityQueueNodeState>> pq; //smallest elements will apear on front 

    std::vector<int> pom_path;
    int pom_cost;
    int pom_bound;

    findCheapest();

    std::vector<bool> visited(nodesNumber, false);
    visited[start_node] = true;
    int initial_bound = lowerBound(visited, start_node);
    pq.emplace(PiorityQueueNodeState(start_node, std::vector<int>{start_node}, 0, initial_bound, 0));

    while (!pq.empty()) {
        PiorityQueueNodeState current = pq.top();
        pq.pop();

        if (current.lower_bound > minPathCost) {
            continue;
        }

        //Check for Hamiltonian Cyrcle
        if (current.level == nodesNumber - 1) {
            int return_cost = matrix[current.node][start_node];
            if (return_cost != -1) {
                int final_cost = current.cost + return_cost;
                if (final_cost <= minPathCost) {
                    minPathCost = final_cost;
                    bestPath = current.path;
                    bestPath.push_back(start_node);
                }
            }
            continue;
        }

        std::vector<bool> visited(nodesNumber, false);
        for (int node : current.path) {
            visited[node] = true;
        }

        // Explore the neighbors (next level)
        for (int next_vertex = 0; next_vertex < nodesNumber; ++next_vertex) {

            if (matrix[current.node][next_vertex] == -1 || visited[next_vertex]) {
                continue;
            }

            // Calculate new cost
            pom_cost = current.cost + matrix[current.node][next_vertex];

            if (pom_cost > minPathCost) {
                continue;
            }

            pom_path = current.path;
            pom_path.push_back(next_vertex);

			// Calculate the lower bound
            pom_bound = pom_cost + lowerBound(visited, next_vertex);
			
            // If the lower bound is less than the current minimum path cost, add the node to the queue
            if (pom_bound < minPathCost) {
                pq.push(PiorityQueueNodeState(next_vertex, pom_path, pom_cost, pom_bound, current.level + 1));
            }
        }
    }

	result.bestPath = bestPath;
	result.minPathCost = minPathCost;

    return result;
}
TSP_Result BB::findCheapestHamiltonianCircle_BFS(int start_node, std::optional<int> upper_limit) {
    if (start_node > nodesNumber - 1 || start_node < 0) {
        throw std::invalid_argument("Invalid start node");
    }

    struct QueueNodeState {
        int node;
        int cost;
        std::vector<int> path;
        std::vector<bool> visited;
    };

    TSP_Result result;
	findCheapest();

    std::queue<QueueNodeState> q;


    std::vector<bool> visited_start_node(nodesNumber, false);
    visited_start_node[start_node] = true;
    q.emplace(start_node, 0, std::vector<int>{start_node}, visited_start_node);

    minPathCost = upper_limit.value_or(INT_MAX);

    std::vector<std::vector<int>> minCost(nodesNumber, std::vector<int>(1 << nodesNumber, INT_MAX));

    minCost[0][1] = 0;

    while (!q.empty()) {
        QueueNodeState current = q.front();
        q.pop();

        bool allVisited = true;
        for (bool visited : current.visited) {
            if (!visited) {
                allVisited = false;
                break;
            }
        }

        if (allVisited) {
            int return_cost = matrix[current.node][start_node];
            if (return_cost != -1) {
                int final_cost = current.cost + return_cost;
                if (final_cost <= minPathCost) {
                    minPathCost = final_cost;
                    bestPath = current.path;
                    bestPath.push_back(start_node);
                }
            }
            continue;
        }

        for (int next = 0; next < nodesNumber; next++) {
            if (!current.visited[next] && matrix[current.node][next] != -1) {

                int nextCost = current.cost + matrix[current.node][next];
				if (nextCost > minPathCost) {
					continue;
				}
                std::vector<bool> next_visited = current.visited;
                next_visited[next] = true;

				int next_appriximation = nextCost + lowerBound(next_visited, next);
                if (next_appriximation <= minPathCost) {
                    std::vector<int> next_path = current.path;
                    next_path.push_back(next);
                    q.emplace(next, nextCost, next_path, next_visited);
                }
            }
        }
    }



	result.bestPath = bestPath;
	result.minPathCost = minPathCost;
    return result;
}

void BB::req_findCheapestHamiltonianCircle(std::vector<int>& path, std::vector<bool>& visited, int current, int currentCost) {

    if (path.size() == nodesNumber) {
        if (matrix[path.back()][path[0]] != -1) {
            int totalCost = currentCost + matrix[path.back()][path[0]];
            if (totalCost <= minPathCost) {
                minPathCost = totalCost;
                bestPath = path;
                bestPath.push_back(path[0]);
            }
        }
        return;
    }

    for (int next = 0; next < nodesNumber; ++next) {

        if (!visited[next] && matrix[current][next] != -1) {
            int newCost = currentCost + matrix[current][next];
            if (newCost >= minPathCost) {
                continue;
            }
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
    int lower_bound = 0;
    for (int i = 0; i < nodesNumber; i++) {
        if (!visited[i] && i != current_node) {
            lower_bound += min_edge_cost[i];
        }
    }
    return lower_bound;
}
void BB::findCheapest() {
    min_edge_cost = std::vector<int>(nodesNumber, INT_MAX);
    for (int i = 0; i < nodesNumber; i++) {      
        for (int j = 0; j < nodesNumber; j++) {  
            if (matrix[i][j] != -1 && min_edge_cost[i] > matrix[i][j]) {
                min_edge_cost[i] = matrix[i][j];
            }
        }
    }    
}
