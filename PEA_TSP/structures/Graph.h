#pragma once
#include <iostream>
#include <filesystem>


class Graph {
private:
    int **v; 
    int nodesNumber;
    int knownMinPathCost = INT_MAX;
    bool isComplete();

public:
    Graph(); // constructor
    bool loadFromFile(std::string); // load graph from file
    int getNodesNumber(); // get number of nodes
    int getKnownMinPathCost();
	void setKnownMinPathCost(int); // set known minimum path cost
    void printMatrix(); // print matrix
    int** getMatrix(); // get matrix
    
};

