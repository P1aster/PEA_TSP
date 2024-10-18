#pragma once
#include <iostream>

class Graph {
private:
    int **v; 
    int nodesNumber;
    bool isComplete();

public:
    Graph(); // constructor
    bool loadFromFile(std::string); // load graph from file
    int getNodesNumber(); // get number of nodes
    void printMatrix(); // print matrix
    int** getMatrix(); // get matrix
    
};

