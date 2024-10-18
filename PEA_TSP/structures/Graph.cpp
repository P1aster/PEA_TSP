#pragma once

#include "Graph.h"
#include <iostream>
#include <fstream>
#include <string>

Graph::Graph() {
    nodesNumber = 0;
    v = nullptr;
}

int Graph::getNodesNumber() {
    return nodesNumber;
}

int** Graph::getMatrix() {
    return v;
}

bool Graph::isComplete() {
    for (int i = 0; i < nodesNumber; ++i) {
        for (int j = 0; j < nodesNumber; ++j) {
            if (i != j && (v[i][j] == 0 || v[i][j] == -1)) {
                return false;
            }
        }
    }
    return true;
}

void Graph::printMatrix() {
    for (int i = 0; i < nodesNumber; ++i) {
        for (int j = 0; j < nodesNumber; ++j) {
            std::cout << v[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

bool Graph::loadFromFile(std::string filename) {
    std::ifstream file(filename);


    if (!file) {
        return false;
    }
    file >> nodesNumber;

    Graph::v = new int* [nodesNumber];
    for (int i = 0; i < nodesNumber; i++) {
        v[i] = new int[nodesNumber];
        for (int j = 0; j < nodesNumber; j++) {
            file >> v[i][j];
        }

    }
    file.close();
    return true;
}