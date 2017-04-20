#include <cmath>

#include "function.hpp"

using namespace std;

double Tanh(double x) {
	return tanh(x);
}

double Sigmoid(double x) {
	return (0.5 * tanh(x) + 0.5);
}