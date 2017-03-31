#ifndef HEADER_NODE
#define HEADER_NODE

#include <Eigen/Dense>
#include <vector>

#include "operation.hpp"

class Node {

private:

	OperationType operation;

	std::vector<Node*> inputNode;
	std::vector<int> inputId;
	std::vector<int> paramId;


public:

	Node(OperationType o);

	void addInput(Node* n);
	void addInput(int j);

	void addParam(int j);
};

#endif //HEADER_NODE