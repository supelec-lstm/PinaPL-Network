#ifndef HEADER_NETWORK
#define HEADER_NETWORK

#include <Eigen/Dense>
#include <vector>

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

    std::vector<int> fusionVector(std::vector<int> a, std::vector<int> b);
    void insertVector(std::vector<int> &a, int b);
    int minFreeVector(std::vector<int> a);

public:

    Network(Graph* graph);
};

#endif //HEADER_NETWORK