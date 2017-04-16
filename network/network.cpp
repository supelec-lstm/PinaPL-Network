#include <vector>
#include <set>
#include <stack>
#include <iostream>
#include <Eigen/Dense>

#include "graph.hpp"
#include "network.hpp"

using namespace std;
using namespace Eigen;

Network::Network(Graph* graph) {

	this->nbInput = graph->getNbInput();
	this->nbOutput = graph->getNbOutput();
	this->nbNode = graph->getNbNode();

	int* inter = this->interMemoryPath(graph);
}

int* Network::interMemoryPath(Graph* graph) {
	int taille = this->nbInput + this->nbNode;

	set<int>* locked = new set<int>[taille];
	set<int> lockedGlobal;
	
	int* inter = new int[taille];
	for(int j = 0; j < taille; j++) {
		inter[j] = -1;
	}

	stack<int> mainStack;
	for(int j = 0; j < taille; j++) {
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

	return inter;
}

int Network::minFreeVector(set<int> a) {
	int j;
	for(j = 0; a.count(j) == 1; j++);
	return j;
}
