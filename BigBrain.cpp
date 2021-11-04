#include "BigBrain.hpp"

matrix<double> bigBrain::del_cost(matrix<double> output, matrix<double> comp) {
	matrix<double> result(output.getRows(), output.getColumns());
	for (size_t i = 0; i < output.getRows(); i++) {
		for (size_t j = 0; j < output.getColumns(); j++) {
			result[i][j] = (comp[i][j] - output[i][j]);
		}
	}
	return result;
}

std::vector<matrix<double>> bigBrain::deltas(matrix<double> output, matrix<double> comp) {
	matrix<double> previous = matrix<double>::dot(del_cost(output, comp), matrix<double>::transform(activations.back(), act::d_sigmoid<double>));
	std::vector<matrix<double>> result{ previous };
	for (size_t i = 2; i <= weights.size(); i++) {
		matrix<double> activations_prime(activations[activations.size() - i].getRows(), activations[activations.size() - i].getRows());
		for (int j = 0; j < activations[activations.size() - i].getRows(); j++) {
			activations_prime[j][j] = matrix<double>::transform(activations[activations.size() - i], act::d_sigmoid<double>)[j][0];
		}
		previous = matrix<double>::cross(activations_prime, matrix<double>::cross(matrix<double>::transpose(weights[weights.size() - i + size_t(1)]), previous));
		result.push_back(previous);
	}
	std::reverse(result.begin(), result.end());
	return result;
	
}

void bigBrain::initialize() {
	std::random_device device;
	std::mt19937 generator(device());
	std::uniform_real_distribution<double> dis(0.0, 1.0);
	for (size_t i = 0; i < weights.size(); i++) {
		for (size_t j = 0; j < weights[i].getRows(); j++) {
			for (size_t k = 0; k < weights[i].getColumns(); k++) {
				weights[i][j][k] = dis(generator);
			}
			biases[i][j][0] = dis(generator);
		}
	}
}

matrix<double> bigBrain::flow(matrix<double> thought, size_t layer) {
	return matrix<double>::add(matrix<double>::cross(weights[layer], thought), biases[layer]);
}

bigBrain::bigBrain(std::vector<size_t> sizes) {
	for (size_t i = 1; i < sizes.size(); i++) {
		weights.push_back(matrix<double>(sizes[i], sizes[i - size_t(1)]));
		biases.push_back(matrix<double>(sizes[i],1,1));
	}
}

matrix<double> bigBrain::think(matrix<double> input) {
	activations.erase(activations.begin(), activations.end());
	matrix<double> output = flow(input, 0);
	activations.push_back(output);
	output.transform(act::sigmoid<double>);
	for (size_t i = 1; i < weights.size(); i++) {
		matrix<double> activation = flow(output, i);
		activations.push_back(activation);
		output = matrix<double>::transform(activation, act::sigmoid<double>);
	}
	return output;
}

void bigBrain::learn(std::vector<matrix<double>> inputs, std::vector<matrix<double>> desired) {
	std::vector<matrix<double>> weightChanges;
	std::vector<matrix<double>> biasChanges;
	for (int i = 0; i < inputs.size(); i++) {
		std::vector<matrix<double>> delta = deltas(think(inputs[i]), desired[i]);
		if (i == 0) { 
			biasChanges = delta;
			for (int j = 0; j < biasChanges.size(); j++) {
				std::cout << activations[j].getRows() << " X " << activations[j].getColumns() << std::endl;
				std::cout << biasChanges[j].getRows() << " X " << biasChanges[j].getColumns() << std::endl;
				matrix<double> activations_prime(activations[activations.size() - i].getRows(), activations[activations.size() - i].getRows());
				for (int j = 0; j < activations[activations.size() - i].getRows(); j++) {
					activations_prime[j][j] = matrix<double>::transform(activations[i], act::sigmoid<double>)[j][0];
				}

				weightChanges.push_back(matrix<double>::cross(activations_prime, biasChanges[j]));

			}
		}
		else {
			for (int j = 0; j < biasChanges.size(); j++) {
				biasChanges[j] = matrix<double>::scalarMult(biasChanges[j], double(i) / i + 1) + matrix<double>::scalarMult(delta[j], double(1) / i + 1);
				weightChanges[j] = matrix<double>::scalarMult(weightChanges[j], double(i) / i + 1) + matrix<double>::scalarMult(matrix<double>::cross(activations[j], delta[j]), double(1) / i + 1);
			}
		}
	}
	for (size_t i = 0; i < weights.size(); i++) {
		biases[i] += biasChanges[i];
		weights[i] += weightChanges[i];
	}
}

void bigBrain::memorize() {

}

void bigBrain::recall(std::string memories) {

}


