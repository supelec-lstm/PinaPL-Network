#ifndef HEADER_GRAPH
#define HEADER_GRAPH

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

class Graph {

private:

	int nbInput;
	int nbOutput;
	int nbNode;

	std::vector<OperationType> operations;
	std::vector<int> nbInputNode;
	std::vector<int*> inputNodes;

	std::vector<int> outputNodes;

	std::vector<int> nbParam;
	std::vector<int*> params;

	void addNode(OperationType operation, int nbEntry, int* entry, int nbParam, int* param);

public:

	Graph(int nbreInput, int nbreOutput);

	void addOutput(int n);
	void addRecursiveOutput(int n, int m);

	void addNodeAddition(int e1, int e2);
	void addNodeAdditionConstant(int e, int p);
	void addNodeProductConstant(int e, int p);
	void addNodeHadamard(int e1, int e2);
	void addNodeHadamardConstant(int e, int p);
	void addNodeSigmoid(int e);
	void addNodeTanh(int e);

	int getNbInput();
	int getNbInput(int j);
	int getNbOutput();
	int getNbNode();
	std::vector<int> getOutput(int j);
};

#endif //HEADER_GRAPH