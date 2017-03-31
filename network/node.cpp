#include <vector>
#include <Eigen/Dense>

#include "operation.hpp"
#include "node.hpp"

using namespace std;
using namespace Eigen;

Node::Node(OperationType o) {
	this->operation = o;
} 

Node::addInput(Node* node) {
	this->inputNode.push_back(node);
}

Node::addInput(int j) {
	this->inputId.push_back(j);
}

Node::addParam(int j) {
	this->paramId.push_back(j);
}