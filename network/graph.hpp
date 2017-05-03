#ifndef HEADER_GRAPH
#define HEADER_GRAPH

#include <Eigen/Dense>
#include <vector>

enum OperationType {
	NO_OPERATION,
    ADDITION,
    ADDITION_CONSTANT,
    PRODUCT_CONSTANT,
    PRODUCT_REVERSE,
    HADAMARD,
    HADAMARD_CONSTANT,
    HADAMARD_REVERSE,+-
    HADAMARD_MEMORY,
    SIGMOID,
    SIGMOID_DERIVATE,
    TANH,
    TANH_DERIVATE
};

struct DimensionalError : public std::exception {
	const char * what () const throw () {
		return "Mauvais dimensionnement";
	}
};

struct Node {
	int size;
	OperationType operation;
	int nbInput;
	std::vector<int> input;
	int nbOutput;
	std::vector<int> output;
	int nbParam;
	std::vector<int> param;
	bool isOutput;
	bool isMemorized;
	int memoryNumber;
};

struct VectorParam {
	int size;
};

struct MatrixParam {
	int sizeX;
	int sizeY;
};

class Graph {

private:

	int nbInput;
	int nbOutput;
	int nbNode;
	int nbVectorParams;
	int nbMatrixParams;

	std::vector<Node> nodes;
	std::vector<int> output;

	std::vector<VectorParam> vectorParams;
	std::vector<MatrixParam> matrixParams;

	int addNode(OperationType operation, int nbEntry, int* entry, int nbParam, int* param, int size);

public:

	Graph(int nbInput, std::vector<int> sizeInput, int nbVectorParams, std::vector<int> sizeVectorParams, int nbMatrixParams, std::vector<std::pair<int, int> > sizeMatrixParams);

	void setOutput(int n);

	int addNodeAddition(int e1, int e2);
	int addNodeAdditionConstant(int e, int p);
	int addNodeProductConstant(int e, int p);
	int addNodeHadamard(int e1, int e2);
	int addNodeHadamardConstant(int e, int p);
	int addNodeSigmoid(int e);
	int addNodeTanh(int e);

	int addPerceptron(int x, int m, int b);
	int addGate(int x1, int m1, int x2, int m2, int b);
	int addBlock(int x1, int m1, int x2, int m2, int b);

	int getNbInput();
	int getNbInput(int j);
	int getNbOutput();
	int getNbOutput(int j);
	int getNbParams(int j);
	int getNbNode();
	int getNbVector();
	int getNbMatrix();

	int* getOutput(int j);
	int* getInput(int j);
	int* getParams(int j);

	OperationType getOperation(int j);

	bool isOutput(int j);
	int indexOutput(int j);

	int getSizeNode(int j);
	int getSizeVector(int j);
	int getSizeMatrixRows(int j);
	int getSizeMatrixCols(int j);
};

#endif //HEADER_GRAPH