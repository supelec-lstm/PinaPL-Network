#include <Eigen/Dense>
#include <iostream>

#include "network/network.hpp"

int main(int argc, char **argv) {
    Graph* graph = new Graph(1, 1);
    graph->addNodeProductConstant(0, 0);
    graph->addNodeSigmoid(1);

    Network* network = new Network(graph);
}