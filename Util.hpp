#pragma once
#include<iostream>
#include<algorithm>
#include<numeric>
#include<vector>

namespace mod {
	int gcd(int LHS, int RHS);
	int eulerPhi(int NUM);
}

namespace act {
	template <typename T>
	T sigmoid(T value) {
		return 1 / (1 + exp(-value));
	}

	template <typename T>
	T d_sigmoid(T value) {
		T temp = sigmoid(value);
		return temp * (1 - temp);
	}
}