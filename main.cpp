#include <Eigen/Dense>
#include <iostream>

#include "network/network.hpp"

Graph* getLSTMGraph();
Graph* getPerceptronGraph();

int main(int argc, char **argv) {
    Network* network = new Network(getLSTMGraph());
}

Graph* getLSTMGraph() {
	int n = 3;
    Graph* graph = new Graph(n, 2);

    graph->addNodeProductConstant(1, 0);			//0
    graph->addNodeProductConstant(2, 1);			//1
    graph->addNodeAddition(n, n+1);					//2
    graph->addNodeAdditionConstant(n+2, 0);			//3
    graph->addNodeSigmoid(n+3);						//4

    graph->addNodeProductConstant(1, 2);			//5
    graph->addNodeProductConstant(2, 3);			//6
    graph->addNodeAddition(n+5, n+6);				//7
    graph->addNodeAdditionConstant(n+7, 1);			//8
    graph->addNodeSigmoid(n+8);						//9

    graph->addNodeProductConstant(1, 4);			//10
    graph->addNodeProductConstant(2, 5);			//11
    graph->addNodeAddition(n+10, n+11);				//12
    graph->addNodeAdditionConstant(n+12, 2);		//13
    graph->addNodeTanh(n+13);						//14

    graph->addNodeProductConstant(1, 6);			//15
    graph->addNodeProductConstant(2, 7);			//16
    graph->addNodeAddition(n+15, n+16);				//17
    graph->addNodeAdditionConstant(n+17, 3);		//18
    graph->addNodeSigmoid(n+18);					//19

    graph->addNodeHadamard(0, n+4);					//20
    graph->addNodeHadamard(n+9, n+14);				//21
    graph->addNodeAddition(n+20, n+21);				//22

    graph->addNodeTanh(n+22);						//23
    graph->addNodeHadamard(n+23, n+19);				//24

    return graph;
}

Graph* getPerceptronGraph() {
	int n = 2;
	Graph* graph = new Graph(n, 1);

	graph->addNodeProductConstant(1, 0);			//0
    graph->addNodeProductConstant(2, 1);			//1
    graph->addNodeAddition(n, n+1);					//2
    graph->addNodeAdditionConstant(n+2, 0);			//3
    graph->addNodeSigmoid(n+3);						//4

    return graph;
}