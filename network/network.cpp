#include <vector>
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

	vector<vector<int>> locked(taille);
	vector<int> lockedGlobal;
	
	int* inter = new int[taille];
	for(int j = 0; j < taille; j++) {
		inter[j] = -1;
	}

	vector<int> queue;
	for(int j = 0; j < taille; j++) {
		if(graph->getNbOutput(j) == 0) {
			queue.push_back(j);
		}
	}

	while(queue.size() != 0) {

		int j = queue[0];
		queue.erase(queue.begin());

		if(inter[j] == -1) {
			vector<int> interQueue;

			// Determination of the value of the branch
			vector<int> interLocked;
			int* outputMain = graph->getOutput(j);
			int m = graph->getNbOutput(j);

			for(int i = 0; i < m; i++) {
				if(inter[outputMain[i]] != -1) {
					interLocked = this->fusionVector(interLocked, locked[outputMain[i]]);
				}
			}

			int k = this->minFreeVector(this->fusionVector(lockedGlobal, interLocked));
			this->insertVector(interLocked, k);

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
					this->insertVector(lockedGlobal, k);
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
						interQueue.push_back(input[i]);
					}
				}
			}

			queue.insert(queue.begin(), interQueue.begin(), interQueue.end());
		}
	}

	return inter;
}

vector<int> Network::fusionVector(vector<int> a, vector<int> b) {
	vector<int> res;
	int i = 0;
	int j = 0;
	while(i < a.size() && j < b.size()) {
		if(a[i] < b[j]) {
			res.push_back(a[i]);
			i++;
		} else if(a[i] > b[j]) {
			res.push_back(b[j]);
			j++;
		} else {
			res.push_back(a[i]);
			i++;
			j++;
		}
	}
	if(i < a.size()) {
		res.insert(res.end(), a.begin() + i, a.end());
	} else if(j < b.size()) {
		res.insert(res.end(), b.begin() + j, b.end());
	}
	return res;
}

void Network::insertVector(vector<int> &a, int b) {
	int i;
	for(i = 0; i < a.size(); i++) {
		if(a[i] > b) {
			a.insert(a.begin() + i, b);
			break;
		}
		else if(a[i] == b) {
			break;
		}
	}
	if(i == a.size()) {
		a.push_back(b);
	}
}

int Network::minFreeVector(std::vector<int> a) {
	for(int i = 0; i < a.size(); i++) {
		cout << a[i] << " ";
	}
	int j;
	for(j = 0; j < a.size() && j == a[j]; j++);
	cout << " : " << j << endl;
	return j;
}
