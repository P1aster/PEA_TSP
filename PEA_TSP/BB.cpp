#include "BB.h"


BB::BB(Graph graph) {
    this->nodesNumber = graph.getNodesNumber();
    this->matrix = graph.getMatrix();
}
