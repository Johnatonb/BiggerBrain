#pragma once
#include "Matrix.hpp"
#include "Util.hpp"
#include <random>

class bigBrain {
private:
	std::vector<matrix<double>> weights;
	std::vector<matrix<double>> biases;

	std::vector<matrix<double>> activations;

	static matrix<double> del_cost(matrix<double> output, matrix<double> comp);

	std::vector<matrix<double>> deltas(matrix<double> output, matrix<double> comp);

	matrix<double> flow(matrix<double> thought, size_t layer);

public:
	bigBrain(std::vector<size_t> sizes);

	void initialize();

	matrix<double> think(matrix<double> input);

	void learn(std::vector<matrix<double>> inputs, std::vector<matrix<double>> desired);

	void memorize();

	void recall(std::string memories);

};