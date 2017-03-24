#ifndef HEADER_NETWORK
#define HEADER_NETWORK

#include <Eigen/Dense>
#include <vector>

#include "operation.hpp"

struct Memory {
    std::vector<Eigen::VectorXd> input;
    std::vector<Eigen::VectorXd> output;
    std::vector<Eigen::VectorXd> memory;
};

class Network {

private:

    int nbreInput;
    int nbreOutput;
    int nbreRecursiveOutput;

    int nbreNode;
    int nbreVectorConstant;
    int nbreMatrixConstant;

    int** relation;

    Network* reverseNetwork;

    Eigen::VectorXd* constantVector;
    Eigen::MatrixXd* constantMatrix;

public:

    Network(int nbreInput, int nbreOutput, int nbreRecursiveOutput, int nbreNode, int nbreVectorConstant, int nbreMatrixConstant);

    void init(int** relation, OperationType* operations);
    void init(std::vector<int> relation, OperationType* operations);

    void verify();

    void setReverseNetwork();
};

#endif //HEADER_NETWORK