#include <Eigen/Dense>
#include <iostream>
#include <vector>

#define NLOG
#define FILE_NAME "graph.cpp\t\t"

#include "../log.hpp"
#include "graph.hpp"

using namespace std;
using namespace Eigen;

Graph::Graph(int nbInput, vector<int> sizeInput, int nbVectorParams, vector<int> sizeVectorParams, int nbMatrixParams, vector<pair<int, int> > sizeMatrixParams) {
	PRINT_BEGIN_FUNCTION("Graph Constructor")

	this->nbInput = nbInput;
	this->nbOutput = 0;
	this->nbNode = nbInput;
	this->nbVectorParams = nbVectorParams;
	this->nbMatrixParams = nbMatrixParams;

	for(int i = 0; i < this->nbInput; i++){
		Node node;
		node.size = sizeInput[i];
		node.nbInput = 0;
		node.nbOutput = 0;
		node.nbParam = 0;
		node.isOutput = false;
		node.operation = NO_OPERATION;
		this->nodes.push_back(node);
	}

	for(int i = 0; i < this->nbVectorParams; i++) {
		VectorParam param;
		param.size = sizeVectorParams[i];
		this->vectorParams.push_back(param);
	}

	for(int i = 0; i < this->nbMatrixParams; i++) {
		MatrixParam param;
		param.sizeX = sizeMatrixParams[i].first;
		param.sizeY = sizeMatrixParams[i].second;
		this->matrixParams.push_back(param);
	}
	PRINT_END_FUNCTION()
}

int Graph::addNode(OperationType operation, int nbInput, int* input, int nbParam, int* param, int size) {
	PRINT_BEGIN_FUNCTION("addNode")

	Node node;
	node.size = size;
	node.operation = operation;
	node.nbInput = nbInput;
	for(int i = 0; i < nbInput; i++) {
		node.input.push_back(input[i]);
		this->nodes[input[i]].nbOutput++;
		this->nodes[input[i]].output.push_back(this->nbNode);
	}
	node.nbOutput = 0;
	node.nbParam = nbParam;
	for(int i = 0; i < nbParam; i++) {
		node.param.push_back(param[i]);
	}
	node.isOutput = false;
	this->nodes.push_back(node);
	PRINT_END_FUNCTION()
	return this->nbNode++;
}

int Graph::addNodeAddition(int e1, int e2) {
	PRINT_BEGIN_FUNCTION("addNodeAddition")

	int s1 = this->getSizeNode(e1);
	int s2 = this->getSizeNode(e2);
	if(s1 != s2) {
		throw DimensionalError();
	}
	int* entry = new int[2];
	entry[0] = e1;
	entry[1] = e2;

	PRINT_END_FUNCTION()
	return this->addNode(ADDITION, 2, entry, 0, nullptr, s1);
}

int Graph::addNodeAdditionConstant(int e, int p) {
	PRINT_BEGIN_FUNCTION("addNodeAdditionConstant")

	int s1 = this->getSizeNode(e);
	int s2 = this->getSizeVector(p);
	if(s1 != s2) {
		throw DimensionalError();
	}
	int* entry = new int[1];
	entry[0] = e;
	int* param = new int[1];
	param[0] = p;

	PRINT_END_FUNCTION()
	return this->addNode(ADDITION_CONSTANT, 1, entry, 1, param, s1);
}

int Graph::addNodeProductConstant(int e, int p) {
	PRINT_BEGIN_FUNCTION("addNodeProductConstant")

	int s1 = this->getSizeNode(e);
	int s2 = this->getSizeMatrixCols(p);
	if(s1 != s2) {
		throw DimensionalError();
	}
	int* entry = new int[1];
	entry[0] = e;
	int* param = new int[1];
	param[0] = p;

	PRINT_END_FUNCTION()
	return this->addNode(PRODUCT_CONSTANT, 1, entry, 1, param, this->getSizeMatrixRows(p));
}

int Graph::addNodeHadamard(int e1, int e2) {
	PRINT_BEGIN_FUNCTION("addNodeHadamard")

	int s1 = this->getSizeNode(e1);
	int s2 = this->getSizeNode(e2);
	if(s1 != s2) {
		throw DimensionalError();
	}
	int* entry = new int[2];
	entry[0] = e1;
	entry[1] = e2;

	PRINT_END_FUNCTION()
	return this->addNode(HADAMARD, 2, entry, 0, nullptr, s1);
}

int Graph::addNodeHadamardConstant(int e, int p) {
	PRINT_BEGIN_FUNCTION("addNodeHadamardConstant")

	int s1 = this->getSizeNode(e);
	int s2 = this->getSizeVector(p);
	if(s1 != s2) {
		throw DimensionalError();
	}
	int* entry = new int[1];
	entry[0] = e;
	int* param = new int[1];
	param[0] = p;

	PRINT_END_FUNCTION()
	return this->addNode(HADAMARD_CONSTANT, 1, entry, 1, param, s1);
}

int Graph::addNodeSigmoid(int e) {
	PRINT_BEGIN_FUNCTION("addNodeSigmoid")

	int s = this->getSizeNode(e);
	int* entry = new int[1];
	entry[0] = e;

	PRINT_END_FUNCTION()
	return this->addNode(SIGMOID, 1, entry, 0, nullptr, s);
}

int Graph::addNodeTanh(int e) {
	PRINT_BEGIN_FUNCTION("addNodeTanh")

	int s = this->getSizeNode(e);
	int* entry = new int[1];
	entry[0] = e;

	PRINT_END_FUNCTION()
	return this->addNode(TANH, 1, entry, 0, nullptr, s);
}

int Graph::addPerceptron(int x, int m, int b) {
	PRINT_BEGIN_FUNCTION("addPerceptron")

	int p = this->addNodeProductConstant(x, m);
	int y = this->addNodeAdditionConstant(p, b);

	PRINT_END_FUNCTION()
	return this->addNodeSigmoid(y);
}

int Graph::addGate(int x1, int m1, int x2, int m2, int b) {
	PRINT_BEGIN_FUNCTION("addGate")

	int p1 = this->addNodeProductConstant(x1, m1);
	int p2 = this->addNodeProductConstant(x2, m2);
	int y = this->addNodeAdditionConstant(this->addNodeAddition(p1, p2), b);

	PRINT_END_FUNCTION()
	return this->addNodeSigmoid(y);
}

int Graph::addBlock(int x1, int m1, int x2, int m2, int b) {
	PRINT_BEGIN_FUNCTION("addBlock")

	int p1 = this->addNodeProductConstant(x1, m1);
	int p2 = this->addNodeProductConstant(x2, m2);
	int y = this->addNodeAdditionConstant(this->addNodeAddition(p1, p2), b);

	PRINT_END_FUNCTION()
	return this->addNodeTanh(y);
}

void Graph::setOutput(int n) {
	PRINT_BEGIN_FUNCTION("setOutput")

	this->nodes[n].isOutput = true;
	this->output.push_back(this->nodes[n]);
	this->nbOutput++;
	PRINT_END_FUNCTION()
}

int Graph::getNbInput() {
	PRINT_BEGIN_FUNCTION("getNbInput")
	PRINT_END_FUNCTION()
	return this->nbInput;
}

int Graph::getNbInput(int j) {
	PRINT_BEGIN_FUNCTION("getNbInput")
	PRINT_END_FUNCTION()
	return this->nodes[j].nbInput;
}

int Graph::getNbOutput() {
	PRINT_BEGIN_FUNCTION("getNbOutput")
	PRINT_END_FUNCTION()
	return this->nbOutput;
}

int Graph::getNbOutput(int j) {
	PRINT_BEGIN_FUNCTION("getNbOutput")
	PRINT_END_FUNCTION()
	return this->nodes[j].nbOutput;
}

int Graph::getNbParams(int j) {
	PRINT_BEGIN_FUNCTION("getNbParams")
	PRINT_END_FUNCTION()
	return this->nodes[j].nbParam;
}

int Graph::getNbNode() {
	PRINT_BEGIN_FUNCTION("getNbNode")
	PRINT_END_FUNCTION()
	return this->nbNode;
}

int Graph::getNbVector() {
	PRINT_BEGIN_FUNCTION("getNbVector")
	PRINT_END_FUNCTION()
	return this->nbVectorParams;
}

int Graph::getNbMatrix() {
	PRINT_BEGIN_FUNCTION("getNbMatrix")
	PRINT_END_FUNCTION()
	return this->nbMatrixParams;
}

int* Graph::getOutput(int j) {
	PRINT_BEGIN_FUNCTION("getNbOutput")
	PRINT_END_FUNCTION()
	int n = this->getNbOutput(j);
	int* res = new int[n];
	for(int i = 0; i < n; i++) {
		res[i] = this->nodes[j].output[i];
	}
	return res;
}

int* Graph::getInput(int j) {
	PRINT_BEGIN_FUNCTION("getInput")
	PRINT_END_FUNCTION()
	int n = this->getNbInput(j);
	int* res = new int[n];
	for(int i = 0; i < n; i++) {
		res[i] = this->nodes[j].input[i];
	}
	return res;
}

int* Graph::getParams(int j) {
	PRINT_BEGIN_FUNCTION("getParams")
	PRINT_END_FUNCTION()
	int n = this->getNbParams(j);
	int* res = new int[n];
	for(int i = 0; i < n; i++) {
		res[i] = this->nodes[j].param[i];
	}
	return res;
}

OperationType Graph::getOperation(int j) {
	PRINT_BEGIN_FUNCTION("getOperations")
	PRINT_END_FUNCTION()
	if(j >= this->nbInput) {
		return this->nodes[j].operation;
	}
}

int Graph::getSizeNode(int j) {
	PRINT_BEGIN_FUNCTION("getSizeNode")
	PRINT_END_FUNCTION()
	return this->nodes[j].size;
}

int Graph::getSizeVector(int j) {
	PRINT_BEGIN_FUNCTION("getSizeVector")
	PRINT_END_FUNCTION()
	return this->vectorParams[j].size;
}

int Graph::getSizeMatrixRows(int j) {
	PRINT_BEGIN_FUNCTION("getSizeMatrixRows")
	PRINT_END_FUNCTION()
	return this->matrixParams[j].sizeX;
}

int Graph::getSizeMatrixCols(int j) {
	PRINT_BEGIN_FUNCTION("getSizeMatrixCols")
	PRINT_END_FUNCTION()
	return this->matrixParams[j].sizeY;
}