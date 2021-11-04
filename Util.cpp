#include "Util.hpp"

namespace mod {
	int gcd(int LHS, int RHS) {
		if (RHS > LHS) {
			std::swap(LHS, RHS);
		}
		while (RHS != 1) {
			int tempLHS = RHS;
			RHS = LHS % RHS;
			if (RHS == 0) {
				return tempLHS;
			}
			LHS = tempLHS;
		}
		return RHS;
	}

	int eulerPhi(int NUM) {
		std::vector<int> comparisonVec(100);
		std::iota(comparisonVec.begin(), comparisonVec.end(), 1);
		return std::count_if(comparisonVec.begin(), comparisonVec.end(), [&](int i) {return gcd(NUM, i) == 1; });
	}
}

