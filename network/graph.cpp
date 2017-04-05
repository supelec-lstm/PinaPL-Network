#include <Eigen/Dense>
#include <iostream>
#include <vector>

#include "graph.hpp"

using namespace std;
using namespace Eigen;

Graph::Graph(int nbInput, int nbOutput) {
	this->nbInput = nbInput;
	this->nbOutput = nbOutput;
	this->nbNode = 0;
}

void Graph::addNode(OperationType operation, int nbInput, int* input, int nbParam, int* param) {

	this->operations.push_back(operation);
	this->nbInputNode.push_back(nbInput);
	this->inputNodes.push_back(input);
	this->nbParam.push_back(nbParam);
	this->params.push_back(param);

	this->nbNode++;
}

void Graph::addNodeAddition(int e1, int e2) {
	int* entry = new int[2];
	entry[0] = e1;
	entry[1] = e2;
	this->addNode(ADDITION, 2, entry, 0, nullptr);
}

void Graph::addNodeAdditionConstant(int e, int p) {
	int* entry = new int[1];
	entry[0] = e;
	int* param = new int[1];
	param[0] = p;
	this->addNode(ADDITION_CONSTANT, 1, entry, 1, param);
}

void Graph::addNodeProductConstant(int e, int p) {
	int* entry = new int[1];
	entry[0] = e;
	int* param = new int[1];
	param[0] = p;
	this->addNode(PRODUCT_CONSTANT, 1, entry, 1, param);
}

void Graph::addNodeHadamard(int e1, int e2) {
	int* entry = new int[2];
	entry[0] = e1;
	entry[1] = e2;
	this->addNode(HADAMARD, 2, entry, 0, nullptr);
}

void Graph::addNodeHadamardConstant(int e, int p) {
	int* entry = new int[1];
	entry[0] = e;
	int* param = new int[1];
	param[0] = p;
	this->addNode(HADAMARD_CONSTANT, 1, entry, 1, param);
}

void Graph::addNodeSigmoid(int e) {
	int* entry = new int[1];
	entry[0] = e;
	this->addNode(SIGMOID, 1, entry, 0, nullptr);
}

void Graph::addNodeTanh(int e) {
	int* entry = new int[1];
	entry[0] = e;
	this->addNode(TANH, 1, entry, 0, nullptr);
}

void Graph::addOutput(int n) {
	this->outputNodes.push_back(n);
}

int Graph::getNbInput() {
	return this->nbInput;
}

int Graph::getNbInput(int j) {
	return this->nbInputNode[j];
}

int Graph::getNbOutput() {
	return this->nbOutput;
}

int Graph::getNbNode() {
	return this->nbNode;
}

vector<int> Graph::getOutput(int j) {
	vector<int> res;
	for(int i = 0; i < this->nbNode; i++) {
		for(int k = 0; k < this->nbInputNode[i]; k++) {
			if(this->inputNodes[i][k] == j) {
				res.push_back(i + this->nbInput);
				break;
			}
		}
	}
	cout << j << " " << res[0] << endl;
	return res;
}