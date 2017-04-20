#include <Eigen/Dense>
#include <iostream>
#include <vector>
#include <ctime>
#include <math.h>

#define LOG
#define FILE_NAME "main.cpp\t\t"

#include "log.hpp"
#include "network/network.hpp"

using namespace std;
using namespace Eigen;

int tab_log = 1;

Graph* getLSTMGraph();
Graph* getPerceptronGraph();
int main(int argc, char **argv) {
	Network* network = new Network(getLSTMGraph());
	VectorXd* input = new VectorXd[3];
	VectorXd v(7);
	v << 1, 1, 1, 1, 1, 1, 1;
	input[0] = v;
	input[1] = v;
	input[2] = v;
	network->forwardCalcul(input);
}

Graph* getLSTMGraph() {
	PRINT_BEGIN_FUNCTION("getLSTMGraph")
	int n = 3;
	vector<int> sizeInput = {7, 7, 7};
	vector<int> sizeVectorParams = {7, 7, 7, 7};
	vector<pair<int, int> > sizeMatrixParams = {{7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}};

    Graph* graph = new Graph(n, sizeInput, 4, sizeVectorParams, 8, sizeMatrixParams);

    int f = graph->addGate(1, 0, 2, 1, 0);
    int i = graph->addGate(1, 2, 2, 3, 1);
    int z = graph->addBlock(1, 4, 2, 5, 2);
    int o = graph->addGate(1, 6, 2, 7, 3);

    int c = graph->addNodeAddition(graph->addNodeHadamard(0, f), graph->addNodeHadamard(i, z));
    int y = graph->addNodeHadamard(graph->addNodeTanh(c), o);

    graph->setOutput(c);
    graph->setOutput(y);

    PRINT_END_FUNCTION()
    return graph;
}

Graph* getPerceptronGraph() {
	PRINT_BEGIN_FUNCTION("getPerceptronGraph")
	int n = 2;
	vector<int> sizeInput = {7, 7, 7};
	vector<int> sizeVectorParams = {7};
	vector<pair<int, int> > sizeMatrixParams = {{7, 7}, {7, 7}};

	Graph* graph = new Graph(n, sizeInput, 1, sizeVectorParams, 2, sizeMatrixParams);	

	int y = graph->addGate(0, 0, 1, 1, 0);

	graph->setOutput(y);

	PRINT_END_FUNCTION()
    return graph;
}