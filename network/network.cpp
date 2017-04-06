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

	bool* marque = new bool[this->nbNode + this->nbInput];
	int* inter = new int[this->nbNode + this->nbInput];
	for(int j = 0; j < this->nbNode + this->nbInput; j++) {
		marque[j] = false;
		inter[j] = -1;
	}

	vector<int> queue;
	for(int j = 0; j < this->nbInput; j++){
		queue.push_back(j);
		marque[j] = true;
		inter[j] = j;
	}

	while(queue.size() != 0) {
		int j = queue[0];
		queue.erase(queue.begin());
		marque[inter[j]] = false;

		vector<int> output = graph->getOutput(j);
		int n = output.size();
		for(int i = 0; i < n; i++) {
			int o = output[i];
			if(inter[o] == -1) {
				int k = 0;
				while(marque[k]) {
					k++;
				}
				marque[k] = true;
				inter[o] = k;
				queue.push_back(o);
			}
		}
	}
}