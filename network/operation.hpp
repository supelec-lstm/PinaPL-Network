#ifndef HEADER_OPERATION
#define HEADER_OPERATION

#include <stdio.h>
#include <stdarg.h>

#include <Eigen/Dense>
#include <vector>

enum OperationType {
    ADDITION,
    ADDITION_CONSTANT,
    PRODUCT_CONSTANT,
    HADAMARD,
    HADAMARD_CONSTANT,
    SIGMOID,
    TANH
};

//typedef Eigen::VectorXd (*UniOperation) (Eigen::VectorXd);
//typedef Eigen::VectorXd (*BiOperation) (Eigen::MatrixXd, Eigen::VectorXd);

Eigen::VectorXd addition(Eigen::VectorXd, Eigen::VectorXd);
Eigen::VectorXd hadamard(Eigen::VectorXd, Eigen::VectorXd);
Eigen::VectorXd product(Eigen::MatrixXd, Eigen::VectorXd);
Eigen::VectorXd sigmoid(Eigen::VectorXd);
Eigen::VectorXd tangent(Eigen::VectorXd);

#endif //HEADER_OPERATION