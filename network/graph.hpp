#ifndef HEADER_NODE
#define HEADER_NODE

#include <Eigen/Dense>
#include <vector>

#include "operation.hpp"
#include "node.hpp"

class Graph {

private:

	int nbreInput;
	int nbreOutput;

	std::vector<Node*> node;


public:

	Graph(OperationType o);
};

#endif //HEADER_NODE