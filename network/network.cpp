#include <vector>
#include <Eigen/Dense>

#include "network.hpp"

using namespace std;
using namespace Eigen;

Network::Network(int nbreInput, int nbreOutput, int nbreRecursiveOutput, int nbreNode, int nbreVectorConstant, int nbreMatrixConstant) {
    this->nbreInput = nbreInput;
    this->nbreOutput = nbreOutput;
    this->nbreRecursiveOutput = nbreRecursiveOutput;

    this->nbreNode = nbreNode;
    this->nbreVectorConstant = nbreVectorConstant;
    this->nbreMatrixConstant = nbreMatrixConstant;

    this->constantVector = new VectorXd[this->nbreVectorConstant];
    this->constantMatrix = new MatrixXd[this->nbreMatrixConstant];


}