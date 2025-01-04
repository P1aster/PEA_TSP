#include "Path.h"

Path::Path() {
	this->size = 0;
	this->cost = 0;
}

Path::Path(int size) {
    this->size = size;
    this->cost = 0;
}

Path::~Path() {}


int Path::calculateCost(Graph g) {
    int sum = 0;

    for (int i = 0; i < size; i++) {
        int curr = nodesList[i];
        int next = nodesList[(i + 1) % size];
        int value = g.getMatrix()[curr][next];

        sum += value;
    }

    cost = sum;
    return cost;
}

std::string Path::to_string() {

    std::string output;

    for (int i = 0; i < size; i++) {
        output += std::to_string(nodesList[i]) + ", ";
    }

    output += std::to_string(nodesList[0]);

    return output;
}


void Path::setNodesList(std::vector<int> newList) {
	nodesList = newList;
}

std::vector<int> Path::getNodesList() const {
    return nodesList;
}


int Path::getCost() const {
    return cost;
}

void Path::setCost(int cost) {
    Path::cost = cost;
}

int Path::getSize() const {
    return size;
}

void Path::setSize(int size) {
    Path::size = size;
}