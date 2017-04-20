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
    std::function<void()>* forward;

    std::vector<std::vector<int> > getPath(Graph* graph);
    int* getInterMemoryPath(Graph* graph);
    void fillActionFunction(Graph* graph, int* inter);
    std::function<void()> getFunction(Graph* graph, int* inter, int node);

    int minFreeVector(std::set<int> a);

public:

    Network(Graph* graph);

    Eigen::VectorXd* forwardCalcul(Eigen::VectorXd* input);
};

#endif //HEADER_NETWORK