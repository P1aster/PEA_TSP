#pragma once
#include <vector>
#include "structures/Graph.h"


class Path {

    std::vector<int> nodesList;
    int cost;
    int size;

public:
    Path(int size);
    Path();
    ~Path();

    std::vector<int> getNodesList() const;
    void setNodesList(std::vector<int> citiesList);

    int calculateCost(Graph graph);

    int getSize() const;
    int getCost() const;
    void setSize(int size);
    void setCost(int cost); 

    std::string to_string();

};

