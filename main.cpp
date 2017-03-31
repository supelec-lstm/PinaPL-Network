#include <Eigen/Dense>
#include <iostream>

//#include "network/network.hpp"

int main(int argc, char **argv) {
    int x = 0;
    auto lambda = [&x]() {
    	std::cout << x;
    };
    lambda();
    x = 1;
    lambda();
}