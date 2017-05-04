#ifndef HEADER_NETWORK
#define HEADER_NETWORK

#include <Eigen/Dense>
#include <vector>
#include <set>

#include "graph.hpp"

struct Memory {
    Eigen::VectorXd* input;
    Eigen::VectorXd* output;
    Eigen::VectorXd* memory;
};

class Network {

private:

    int nbInput;
    int nbOutput;
    int nbNode;
    int nbVector;
    int nbMatrix;

    int nbInter;

    Eigen::VectorXd* interOutput;
    Eigen::VectorXd* interInput;
    Eigen::VectorXd* interResult;
    Eigen::VectorXd* vectorParams;
    Eigen::MatrixXd* matrixParams;

    int nbForward;
    std::vector<std::function<void()> > forward;

    void fillOrder(int* inter, int* order, Graph* graph);
    void fillActionFunction(int* inter, int* order, Graph* graph);

    Graph* getReverseGraph(Graph* graph);

public:

    Network(Graph* graph);

    Eigen::VectorXd* forwardCalcul(Eigen::VectorXd* input);
};

#endif //HEADER_NETWORK