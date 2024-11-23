#pragma once
#include <vector>

struct Node {
    std::vector<int> path;  // Stores the current path
    int cost;               // Cost of the path so far
    int lower_bound;        // Lower bound on the cost to complete this path
    int level;              // Level of the node (depth in tree)
    int current_vertex;     // Current vertex in the path

    Node(int vertex, std::vector<int> path, int cost, int lower_bound, int level) :
        current_vertex(vertex), path(path), cost(cost), lower_bound(lower_bound), level(level) {}

    // Comparator for priority queue
    bool operator>(const Node& other) const {
        return lower_bound > other.lower_bound;
    }
};

