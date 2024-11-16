#pragma once

#include "Graph.h"
#include <iostream>
#include <fstream>
#include <string>

Graph::Graph() {
    this->nodesNumber = 0;
    this->knownMinPathCost = INT_MAX;
    this->v = nullptr;
}

int Graph::getNodesNumber() {
    return nodesNumber;
}

int Graph::getKnownMinPathCost() {
    return knownMinPathCost;
}

void Graph::setKnownMinPathCost(int cost) {
	knownMinPathCost = cost;
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
        std::cerr << "Error: Could not read the file" << std::endl;

        return false;
    }
    std::string line;
    std::string token;


    if (!std::getline(file, line)) {
        std::cerr << "Error: Could not read the first line from the file" << std::endl;
        return false;
    }

    std::istringstream iss(line);


    if (iss >> token) {
        try {
            nodesNumber = std::stoi(token);
        }
        catch (const std::invalid_argument& ) {
            std::cerr << "Error: could`n parse to int" << token << std::endl;
            return false;
        }
    }
    else {
		std::cerr << "Error: Invalid number of nodes" << token << std::endl;
		return false;
    }

    if (iss >> token) {
        try {
            knownMinPathCost = std::stoi(token);
        }
        catch (const std::invalid_argument& ) {
            std::cerr << "Error: could`n parse to int" << token << std::endl;
            return false;
        }
    }
    else {
        knownMinPathCost = INT_MAX;
    }


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