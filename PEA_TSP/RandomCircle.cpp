#include "vector"


bool isValidPath(const std::vector<int>& path, const std::vector<std::vector<int>>& adjMatrix) {
    for (size_t i = 0; i < path.size() - 1; ++i) {
        if (adjMatrix[path[i]][path[i + 1]] == 0) {
            return false;
        }
    }
    return true;
}