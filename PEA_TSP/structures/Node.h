#pragma once
#include <vector>

struct Node {
    std::vector<int> path;  // Stores the current path
    int cost;               // Cost of the path so far
    int lower_bound;        // Lower bound on the cost to complete this path
    uint8_t level;              // Level of the node (depth in tree)
    uint8_t current_vertex;     // Current vertex in the path

    Node(int vertex, std::vector<int> path, int cost, uint8_t lower_bound, uint8_t level) :
        current_vertex(vertex), path(path), cost(cost), lower_bound(lower_bound), level(level) {}

    // Comparator for priority queue to make min-heap based on lower bound (Best-First Search)
    bool operator>(const Node& other) const {
        return lower_bound > other.lower_bound;
    }
};