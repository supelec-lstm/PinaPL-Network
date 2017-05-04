#include <vector>
#include <set>
#include <stack>
#include <iostream>
#include <functional>
#include <cmath>
#include <Eigen/Dense>

#define LOG
#define FILE_NAME "network.cpp\t\t"

#include "../log.hpp"
#include "graph.hpp"
#include "function.hpp"
#include "network.hpp"

using namespace std;
using namespace Eigen;

Network::Network(Graph* graph) {
	PRINT_BEGIN_FUNCTION("Network constructor")

	this->nbInput = graph->getNbInput();
	this->nbOutput = graph->getNbOutput();
	this->nbNode = graph->getNbNode();
	this->nbVector = graph->getNbVector();
	this->nbMatrix = graph->getNbMatrix();

	this->vectorParams = new VectorXd[this->nbVector];
	for(int i = 0; i < this->nbVector; i++) {
		this->vectorParams[i] = VectorXd::Random(graph->getSizeVector(i));
	}

	this->matrixParams = new MatrixXd[this->nbMatrix];
	for(int i = 0; i < this->nbMatrix; i++) {
		this->matrixParams[i] = MatrixXd::Random(graph->getSizeMatrixRows(i), graph->getSizeMatrixCols(i));
	}


	int* inter = new int[this->nbNode];
	int* order = new int[this->nbNode];

	this->fillOrder(inter, order, graph);
	this->nbInter = 0;
	for(int i = 0; i < this->nbNode; i++) {
		if(this->nbInter < inter[i]) {
			this->nbInter = inter[i];
		}
	}
	this->nbInter++;

	this->interOutput = new VectorXd[this->nbInter];
	this->interResult = new VectorXd[this->nbOutput];
	this->fillActionFunction(inter, order, graph);

	PRINT_END_FUNCTION()
}

void Network::fillOrder(int* inter, int* order, Graph* graph) {
	PRINT_BEGIN_FUNCTION("fillOrder")

	//Pre-numerotation of everyNode and sorting

	vector<vector<int>> interOrder(this->nbNode);
	vector<int> maxInter(this->nbNode);

	for(int j = 0; j < this->nbNode; j++) {
		int n = graph->getNbInput(j);
		int* input = graph->getInput(j);
		int m = 1;
		for(int i = 0; i < n; i++) {
			int k = i-1;
			interOrder[j].push_back(input[i]);
			while(k >= 0 && maxInter[interOrder[j][k]] < maxInter[interOrder[j][k+1]]){
				int a = interOrder[j][k];
				interOrder[j][k] = interOrder[j][k+1];
				interOrder[j][k+1] = a;
				k--;
			}
		}
		for(int i = 0; i < n; i++) {
			if(m < maxInter[interOrder[j][i]] + i) {
				m = maxInter[interOrder[j][i]] + i;
			}
		}
		maxInter[j] = m;
	}

	//Initialization

	for(int j = 0; j < this->nbNode; j++) {
		inter[j] = -1;
	}

	int* reverseOrder = new int[this->nbNode];

	stack<int> mainStack;
	for(int j = 0; j < this->nbNode; j++) {
		if(graph->getNbOutput(j) == 0) {
			mainStack.push(j);
		}
	}

	int k = this->nbNode - 1;

	// Main loop

	while(!mainStack.empty()) {
		int j = mainStack.top();
		mainStack.pop();

		// Check if every output of the node is already tagged

		int* output = graph->getOutput(j);
		int n = graph->getNbOutput(j);
		int i, m = 0;
		for(i = 0; i < n & inter[output[i]] != -1; i++) {
			if(reverseOrder[output[i]] > reverseOrder[output[m]]) {
				m = i;
			}
		}
		if(i != n) {
			continue;
		}

		// Set of the order

		order[k] = j;
		reverseOrder[j] = k;

		// Search for the number of the memory node

		set<int> tagged;
		if(n!=0) {
			for(i = k+1; i < reverseOrder[output[m]]; i++) {
				tagged.insert(inter[order[i]]);
			}
		}
		k--;
		int tagNb;
		for(tagNb = 0; tagged.count(tagNb) == 1; tagNb++);
		inter[j] = tagNb;

		// We fill the stack with the input of the node

		for(i = 0; i < interOrder[j].size(); i++){
			mainStack.push(interOrder[j][i]);
		}
	}

	PRINT_END_FUNCTION()
}

void Network::fillActionFunction(int* inter, int* order, Graph* graph) {
	PRINT_BEGIN_FUNCTION("fillActionFunction")
	for(int j = 0; j < this->nbNode; j++) {
		int node = order[j];
		int in = inter[node];
		if(node < this->nbInput) {
			function<void()> func = [node, in, this]() -> void {
				this->interOutput[in] = this->interInput[node];
			};
			this->forward.push_back(func);
		} else {
			int* input = graph->getInput(node);
			int* param = graph->getParams(node);
			int i1;
			int i2;
			function<void()> func;

			switch(graph->getOperation(node)) {
				case ADDITION:
					i1 = inter[input[0]];
					i2 = inter[input[1]];
					func = [i1, i2, in, this]() -> void {
						this->interOutput[in] = this->interOutput[i1] + this->interOutput[i2];
					};
					this->forward.push_back(func);
					break;

			    case ADDITION_CONSTANT:
			    	i1 = inter[input[0]];
			    	i2 = param[0];
			    	func = [i1, i2, in, this]() -> void {
			    		this->interOutput[in] = this->interOutput[i1] + this->vectorParams[i2];
			    	};
			    	this->forward.push_back(func);
					break;
				
			    case PRODUCT_CONSTANT:
			    	i1 = inter[input[0]];
			    	i2 = param[0];
			    	func = [i1, i2, in, this]() -> void {
			    		this->interOutput[in] = this->matrixParams[i2] * this->interOutput[i1];
			    	};
			    	this->forward.push_back(func);
					break;
				
			    case HADAMARD:
			    	i1 = inter[input[0]];
					i2 = inter[input[1]];
					func = [i1, i2, in, this]() -> void {
						this->interOutput[in] = this->interOutput[i1].cwiseProduct(this->interOutput[i2]);
					};
					this->forward.push_back(func);
					break;
				
			    case HADAMARD_CONSTANT:
			    	i1 = inter[input[0]];
			    	i2 = param[0];
			    	func = [i1, i2, in, this]() -> void {
			    		this->interOutput[in] = this->interOutput[i1].cwiseProduct(this->vectorParams[i2]);
			    	};
			    	this->forward.push_back(func);
					break;
				
			    case SIGMOID:
			    	i1 = inter[input[0]];
			    	func = [i1, in, this]() -> void {
			    		this->interOutput[in] = this->interOutput[i1].unaryExpr(&Sigmoid);
			    	};
			    	this->forward.push_back(func);
					break;
				
			    case TANH:
			    	i1 = inter[input[0]];
			    	func = [i1, in, this]() -> void {
			    		this->interOutput[in] = this->interOutput[i1].unaryExpr(&Tanh);
			    	};
			    	this->forward.push_back(func);
					break;
			}
		}
	}
	this->nbForward = this->forward.size();
	PRINT_END_FUNCTION()
}

Graph* Network::getReverseGraph(Graph* graph) {

	//Graph* reverseGraph = new Graph();
}

VectorXd* Network::forwardCalcul(VectorXd* input) {
	PRINT_BEGIN_FUNCTION("forwardCalcul")
	this->interInput = input;
	for(int i = 0; i < this->nbForward; i++) {
		this->forward[i]();
	}
	PRINT_END_FUNCTION()
	return input;
}