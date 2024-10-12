#pragma once
#include "Graph.h"
#include "TSP_Result.h"

class BB {
private:
    int nodesNumber;
    int** matrix;
public:
    BB(Graph graph);
};

