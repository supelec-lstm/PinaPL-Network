#ifndef HEADER_NETWORK
#define HEADER_NETWORK

#include <Eigen/Dense>
#include <vector>
#include <set>

#include "graph.hpp"

struct Memory {
    std::vector<Eigen::VectorXd> input;
    std::vector<Eigen::VectorXd> output;
    std::vector<Eigen::VectorXd> memory;
};

class Network {

private:

    int nbInput;
    int nbOutput;

    int nbNode;

    int* interMemoryPath(Graph* graph);

    int minFreeVector(std::set<int> a);

public:

    Network(Graph* graph);
};

#endif //HEADER_NETWORK