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
    int nbRecursiveOutput;

    int nbNode;
    int nbVectorConstant;
    int nbMatrixConstant;

    int** relation;

    Network* reverseNetwork;

    Eigen::VectorXd* constantVector;
    Eigen::MatrixXd* constantMatrix;

    bool* marque;

    int findMinimum();


public:

    Network(Graph* graph);

    void setReverseNetwork();
};

#endif //HEADER_NETWORK