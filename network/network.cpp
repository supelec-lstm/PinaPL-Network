#include <vector>
#include <iostream>
#include <Eigen/Dense>

#include "graph.hpp"
#include "network.hpp"

using namespace std;

Network::Network(Graph* graph) {

	this->nbInput = graph->getNbInput();
	this->nbOutput = graph->getNbOutput();
	int nbNode = graph->getNbNode();

	bool* marque = new bool[nbNode];
	int* inter = new int[nbNode];
	for(int j = 0; j < nbNode; j++) {
		marque[j] = false;
		inter[j] = -1;
	}

	vector<int> queue;
	for(int j = 0; j < this->nbInput; j++){
		queue.push_back(j);
		marque[j] = false;             //ATTENTION TAILLE : A MODIF
	}

	while(queue.size() != 0) {
		int j = queue[0];
		queue.erase(remove(queue.begin(), queue.end(), j), queue.end());
		marque[inter[j]] = false;
		vector<int> output = graph->getOutput(j);
		int n = output.size();
		for(int i = 0; i < n; i++) {
			if(inter[i] == -1) {
				int k = 0;
				while(marque[k]) {
					k++;
				}
				marque[k] = true;
				inter[i] = k;
				queue.push_back(i);
			}
		}
	}

	for(int j = 0; j < nbNode; j++) {
		cout << inter[j] << " ";
	}
	cout << endl;

}

using namespace std;
using namespace Eigen;