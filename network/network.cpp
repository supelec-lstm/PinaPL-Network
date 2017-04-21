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

	int* inter = this->getInterMemoryPath(graph);
	this->nbInter = 0;
	for(int i = 0; i < this->nbNode; i++) {
		if(this->nbInter < inter[i]) {
			this->nbInter = inter[i];
		}
	}
	this->nbInter++;

	this->interOutput = new VectorXd[this->nbInter];
	this->fillActionFunction(graph, inter);

	PRINT_END_FUNCTION()
}

int* Network::getInterMemoryPath(Graph* graph) {
	PRINT_BEGIN_FUNCTION("getInterMemoryPath")

	int taille = this->nbNode;

	set<int>* locked = new set<int>[this->nbNode];
	set<int> lockedGlobal;
	
	int* inter = new int[this->nbNode];
	for(int j = 0; j < taille; j++) {
		inter[j] = -1;
	}

	stack<int> mainStack;
	for(int j = 0; j < this->nbNode; j++) {
		if(graph->getNbOutput(j) == 0) {
			mainStack.push(j);
		}
	}

	while(mainStack.size() != 0) {

		int j = mainStack.top();
		mainStack.pop();

		if(inter[j] == -1) {
			stack<int> interStack;
			set<int> interLocked;

			// Determination of the value of the branch
			int* outputMain = graph->getOutput(j);
			int m = graph->getNbOutput(j);

			for(int i = 0; i < m; i++) {
				if(inter[outputMain[i]] != -1) {
					interLocked.insert(locked[outputMain[i]].begin(), locked[outputMain[i]].end());
				}
			}

			set<int> finalLocked;
			finalLocked.insert(interLocked.begin(), interLocked.end());
			finalLocked.insert(lockedGlobal.begin(), lockedGlobal.end());

			int k = this->minFreeVector(finalLocked);
			interLocked.insert(k);

			// Main path of the graph
			while(true) {
				locked[j] = interLocked;
				inter[j] = k;

				int i;
				int n;

				int* output = graph->getOutput(j);
				n = graph->getNbOutput(j);

				for(i = 0; i < n && inter[output[i]] != -1; i++);
				if(i != n) {
					lockedGlobal.insert(k);
					break;
				}

				int* input = graph->getInput(j);
				n = graph->getNbInput(j);

				for(i = 0; i < n && inter[input[i]] != -1; i++);
				if(i == n) {
					break;
				}
				else {
					j = input[i];
					i++;
					for(nullptr; i < n; i++){
						interStack.push(input[i]);
					}
				}
			}

			//We fill the main stack with the intermediate stack

			while(!interStack.empty()) {
				mainStack.push(interStack.top());
				interStack.pop();
			}
		}
	}

	PRINT_END_FUNCTION()
	return inter;
}

void Network::fillActionFunction(Graph* graph, int* inter) {
	PRINT_BEGIN_FUNCTION("fillActionFunction")

	this->nbForward = this->nbNode;
	this->forward = new function<void()>[this->nbNode];

	int k = this->nbNode - 1;

	// Main actions

	bool* marqued = new bool[this->nbNode];
	stack<int> mainStack;
	for(int j = 0; j < this->nbNode; j++) {
		if(graph->getNbOutput(j) == 0) {
			mainStack.push(j);
		}
		marqued[j] = false;
	}

	while(mainStack.size() != 0) {

		int j = mainStack.top();
		mainStack.pop();

		if(!marqued[j]) {
			stack<int> interStack;

			// Main path of the graph

			int i;
			int n;

			int* output = graph->getOutput(j);
			n = graph->getNbOutput(j);

			for(i = 0; i < n && marqued[output[i]]; i++);
			if(i == n) {
				PRINT_LOG(j)

				marqued[j] = true;
				this->forward[k] = this->getFunction(graph, inter, j);
				k--;

				int* input = graph->getInput(j);
				n = graph->getNbInput(j);

				for(i = 0; i < n && marqued[input[i]]; i++);
				if(i != n) {
					mainStack.push(input[i]);
					i++;
					for(nullptr; i < n; i++){
						interStack.push(input[i]);
					}

					//We fill the main stack with the intermediate stack

					while(!interStack.empty()) {
						mainStack.push(interStack.top());
						interStack.pop();
					}
				}
			}
		}
	}
	PRINT_END_FUNCTION()
}

function<void()> Network::getFunction(Graph* graph, int* inter, int node) {
	PRINT_BEGIN_FUNCTION("getFunction")

	if(node < this->nbInput) {
		int o = inter[node];
		PRINT_END_FUNCTION()
		return [node, o, this]() -> void {
			this->interOutput[o] = this->interInput[node];
		};
	} else {
		int* input = graph->getInput(node);
		int* param = graph->getParams(node);
		int o = inter[node];
		int i1;
		int i2;
		switch(graph->getOperation(node)) {
			case ADDITION:
				i1 = inter[input[0]];
				i2 = inter[input[1]];
				PRINT_END_FUNCTION()
				return [i1, i2, o, this]() -> void {
					this->interOutput[o] = this->interOutput[i1] + this->interOutput[i2];
				};
				break;

		    case ADDITION_CONSTANT:
		    	i1 = inter[input[0]];
		    	i2 = param[0];
		    	PRINT_END_FUNCTION()
		    	return [i1, i2, o, this]() -> void {
		    		this->interOutput[o] = this->interOutput[i1] + this->vectorParams[i2];
		    	};
				break;
			
		    case PRODUCT_CONSTANT:
		    	i1 = inter[input[0]];
		    	i2 = param[0];
		    	PRINT_END_FUNCTION()
		    	return [i1, i2, o, this]() -> void {
		    		this->interOutput[o] = this->matrixParams[i2] * this->interOutput[i1];
		    	};
				break;
			
		    case HADAMARD:
		    	i1 = inter[input[0]];
				i2 = inter[input[1]];
				PRINT_END_FUNCTION()
				return [i1, i2, o, this]() -> void {
					this->interOutput[o] = this->interOutput[i1].cwiseProduct(this->interOutput[i2]);
				};
				break;
			
		    case HADAMARD_CONSTANT:
		    	i1 = inter[input[0]];
		    	i2 = param[0];
		    	PRINT_END_FUNCTION()
		    	return [i1, i2, o, this]() -> void {
		    		this->interOutput[o] = this->interOutput[i1].cwiseProduct(this->vectorParams[i2]);
		    	};
				break;
			
		    case SIGMOID:
		    	i1 = inter[input[0]];
		    	PRINT_END_FUNCTION()
		    	return [i1, o, this]() -> void {
		    		this->interOutput[o] = this->interOutput[i1].unaryExpr(&Sigmoid);
		    	};
				break;
			
		    case TANH:
		    	i1 = inter[input[0]];
		    	PRINT_END_FUNCTION()
		    	return [i1, o, this]() -> void {
		    		this->interOutput[o] = this->interOutput[i1].unaryExpr(&Tanh);
		    	};
				break;
		}
	}
}

int Network::minFreeVector(set<int> a) {
	PRINT_BEGIN_FUNCTION("minFreeVector")

	int j;
	for(j = 0; a.count(j) == 1; j++);

	PRINT_END_FUNCTION()
	return j;
}

VectorXd* Network::forwardCalcul(VectorXd* input) {
	PRINT_BEGIN_FUNCTION("forwardCalcul")
	this->interInput = input;
	for(int i = 0; i < this->nbForward; i++) {
		this->forward[i]();
		for(int j = 0; j < this->nbInter; j++) {
			PRINT_LOG(this->interOutput[j].transpose());
		}
		PRINT_LOG("------------" + to_string(i) + "----------")
	}
	PRINT_END_FUNCTION()
	return input;
}