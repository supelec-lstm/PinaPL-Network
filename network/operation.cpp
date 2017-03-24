#include <Eigen/Dense>
#include <vector>

#include "operation.hpp"

using namespace std;
using namespace Eigen;

VectorXd addition(VectorXd a1, VectorXd a2){
    return (a1 + a2);
}

VectorXd hadamard(VectorXd a1, VectorXd a2){
    return a1.cwise(a2);
}