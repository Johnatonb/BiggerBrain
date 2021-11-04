#pragma once

#include<vector>
#include<iostream>
#include<exception>
#include<string>

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>

struct sizeException : public std::exception {
	const char * what() const throw () {
		return "Matrix size is invlaid!";
	}
};

struct opperationException : public std::exception {
	const char * what() const throw () {
		return "Matrix sizes are invlaid for this opperation!";
	}
};

template <typename T>
class matrix {
private:
	size_t rows = 0;
	size_t columns = 0;
	T determinate = NULL;
	std::vector<std::vector<T>> base;

	void update();

	void hardCheck(std::string check_type);

	void hardCheck(std::string check_type, matrix<T> comparisonMatrix);

	static void hardCheckMatrix(std::string check_type, matrix<T> checkee);

	static void hardCheckMatrices(std::string check_type, matrix<T> checkee1, matrix<T> checkee2);

public:
	matrix<T>();

	matrix<T>(size_t size);

	matrix<T>(size_t rowCount, size_t columnCount);

	matrix<T>(size_t rowCount, size_t columnCount, T defaultValue);

	std::vector<T> &operator[](size_t rhs);

	matrix<T> operator+(matrix<T> rhs);

	void operator+=(matrix<T> rhs);

	matrix<T> operator+(T rhs);

	void operator+=(T rhs);

	matrix<T> operator-(matrix<T> rhs);

	void operator-=(matrix<T> rhs);

	size_t getRows();

	size_t getColumns();

	bool softCheck(std::string check_type);

	bool softCheck(std::string check_type, matrix<T> comparisonMatrix);

	void pushRow(std::vector<T> addend);

	void pushColumn(std::vector<T> addend);

	void scalarMult(T scalar);

	static matrix<T> scalarMult(matrix<T> multiplicand, T scalar);

	void add(matrix<T> multiplicand);

	void add(T multiplicand);

	static matrix<T> add(matrix<T> multiplicand1, matrix<T> multiplicand2);

	static matrix<T> add(matrix<T> multiplicand1, T multiplicand2);

	void sub(matrix<T> multiplicand);

	static matrix<T> sub(matrix<T> multiplicand1, matrix<T> multiplicand2);
	
	void dot(matrix<T> multiplicand);

	static matrix<T> dot(matrix<T> multiplicand1, matrix<T> multiplicand2);

	void cross(matrix<T> multiplicand);

	static matrix<T> cross(matrix<T> multiplicand1, matrix<T> multiplicand2);

	void transpose();

	static matrix<T> transpose(matrix<T> original);

	void transform(std::function<T(T)> f);

	static matrix<T> transform(matrix <T> original, std::function<T(T)> f);

	cv::Mat visualize();
};

template <typename T>
void matrix<T>::update()
{
	columns = base[0].size();
	rows = base.size();
	hardCheck("SIZE");
}

template <typename T>
void matrix<T>::hardCheck(std::string check_type) {
	try {
		if (check_type == "SIZE") {
			if (base.size() > (int)0) {
				size_t columnsBasis = base[0].size();
				for (size_t i = 0; i < base.size(); i++) {
					if (base[i].size() != columnsBasis) {
						throw sizeException();
					}
				}
			}
		}
		else if (check_type == "DET") {
			if (rows != columns) {
				throw opperationException();
			}
		}
		else if (check_type == "INV") {
			if (rows != columns || determinate == NULL || determinate == 0) {
				throw opperationException();
			}
		}
	}
	catch (sizeException & e) {
		std::cerr << e.what() << std::endl;
		std::exit(-1);
	}
	catch (opperationException & e) {
		std::cerr << e.what() << std::endl;
		std::exit(-1);
	}
}

template <typename T>
void matrix<T>::hardCheck(std::string check_type, matrix<T> comparisonMatrix) {
	try {
		comparisonMatrix.update();
		if (check_type == "DOT" || check_type == "ADD" || check_type == "SUB") {
			if (rows != comparisonMatrix.rows || columns != comparisonMatrix.columns) {
				throw opperationException();
			}
		}
		else if (check_type == "CROSS") {
			if (columns != comparisonMatrix.rows) {
				throw opperationException();
			}
		}
		update();
	}
	catch (opperationException & e) {
		std::cerr << e.what() << std::endl;
		std::exit(-1);
	}
}

template <typename T>
void matrix<T>::hardCheckMatrix(std::string check_type, matrix<T> checkee) {
	try {
		if (check_type == "SIZE") {
			if (checkee.base.size() > (int)0) {
				size_t rowsBasis = checkee.base[0].size();
				for (int i = 1; i < checkee.base.size(); i++) {
					if (checkee.base[i].size() != rowsBasis) {
						throw sizeException();
					}
				}
			}
		}
		else if (check_type == "DET") {
			if (checkee.rows != checkee.columns) {
				throw opperationException();
			}
		}
		else if (check_type == "INV") {
			if (checkee.rows != checkee.columns || checkee.determinate == NULL || checkee.determinate == 0) {
				throw opperationException();
			}
		}
	}
	catch (sizeException & e) {
		std::cerr << e.what() << std::endl;
		std::exit(-1);
	}
	catch (opperationException & e) {
		std::cerr << e.what() << std::endl;
		std::exit(-1);
	}
}

template <typename T>
void matrix<T>::hardCheckMatrices(std::string check_type, matrix<T> checkee1, matrix<T> checkee2) {
	try {
		checkee1.update();
		checkee2.update();
		if (check_type == "DOT" || check_type == "ADD" || check_type == "SUB") {
			if (checkee1.rows != checkee2.rows || checkee1.columns != checkee2.columns) {
				throw opperationException();
			}
		}
		else if (check_type == "CROSS") {
			if (checkee1.columns != checkee2.rows) {
				throw opperationException();
			}
		}
	}
	catch (opperationException & e) {
		std::cerr << e.what() << std::endl;
		std::exit(-1);
	}
}

template <typename T>
matrix<T>::matrix() {

}

template <typename T>
matrix<T>::matrix(size_t size) {
	for (int i = 0; i < size; i++) {
		base.push_back(std::vector<T>(size, double(0)));
		base[i][i] = double(0);
	}
	update();
}

template <typename T>
matrix<T>::matrix(size_t rowCount, size_t columnCount) {
	for (size_t i = 0; i < rowCount; i++) {
		base.push_back(std::vector<T>(columnCount, T(0)));
	}
	update();
}

template <typename T>
matrix<T>::matrix(size_t rowCount, size_t columnCount, T defaultValue) {
	for (size_t i = 0; i < rowCount; i++) {
		base.push_back(std::vector<T>(columnCount, defaultValue));
	}
	update();
}

template <typename T>
std::vector<T>& matrix<T>::operator[](size_t rhs) {
	return base[rhs];
};

template <typename T>
matrix<T> matrix<T>::operator+(matrix<T> rhs) {
	return matrix::add(*this, rhs);
}

template <typename T>
void matrix<T>::operator+=(matrix<T> rhs) {
	this->add(rhs);
}

template <typename T>
matrix<T> matrix<T>::operator+(T rhs) {
	return matrix::add(*this, rhs);
}

template <typename T>
void matrix<T>::operator+=(T rhs) {
	this->add(rhs);
}

template <typename T>
matrix<T> matrix<T>::operator-(matrix<T> rhs) {
	return matrix::sub(*this, rhs);
}

template <typename T>
void matrix<T>::operator-=(matrix<T> rhs) {
	this->sub(rhs);
}

template <typename T>
size_t matrix<T>::getColumns() {
	update();
	return columns;
}

template <typename T>
size_t matrix<T>::getRows() {
	update();
	return rows;
}

template <typename T>
bool matrix<T>::softCheck(std::string check_type) {
	if (check_type == "SIZE") {
		if (base.size() > (int)0) {
			size_t rowsBasis = base[0].size();
			for (int i = 1; i < base.size(); i++) {
				if (base[i].size() != rowsBasis) {
					return false;
				}
			}
		}
	}
	else if (check_type == "DET") {
		if (rows != columns) {
			return false;
		}
	}
	else if (check_type == "INV") {
		if (rows != columns || determinate == NULL || determinate == 0) {
			return false;
		}
	}
	return true;
}

template <typename T>
bool matrix<T>::softCheck(std::string check_type, matrix<T> comparisonMatrix) {
	update();
	if (check_type == "DOT" || check_type == "ADD" || check_type == "SUB") {
		if (rows != comparisonMatrix.getRows() || columns != comparisonMatrix.getColumns()) {
			return false;
		}
	}
	else if (check_type == "CROSS") {
		if (columns != comparisonMatrix.getRows()) {
			return false;
		}
	}
	return true;
}

template <typename T>
void matrix<T>::pushRow(std::vector<T> addend) {
	if (addend.size() == columns) {
		base.push_back(addend);
	}
	update();
}

template <typename T>
void matrix<T>::pushColumn(std::vector<T> addend) {
	if (addend.size() == rows) {
		for (int i = 0; i < rows; i++) {
			base[i].push_back(addend[i]);

		}
	}
	update();
}

template <class T>
void matrix<T>::scalarMult(T scalar) {
	hardCheck("SIZE");
	for_each(base.begin(), base.end(), [&](std::vector<T>& vec) {for_each(vec.begin(), vec.end(), [&](T& i) { i *= scalar; }); });
	update();
}

template <class T>
matrix<T> matrix<T>::scalarMult(matrix<T> multiplicand, T scalar) {
	multiplicand.update();
	matrix::hardCheckMatrix("SIZE", multiplicand);
	for (int i = 0; i < multiplicand.rows; i++) {
		for (int j = 0; j < multiplicand.columns; j++) {
			multiplicand[i][j] *= scalar;
		}
	}
	return multiplicand;
}

template <typename T>
void matrix<T>::add(matrix<T> multiplicand) {
	hardCheck("ADD", multiplicand);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			base[i][j] += multiplicand[i][j];
		}
	}
	update();
}

template <typename T>
void matrix<T>::add(T multiplicand) {
	hardCheck("ADD", multiplicand);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			base[i][j] += multiplicand;
		}
	}
	update();
}

template <typename T>
matrix<T> matrix<T>::add(matrix<T> multiplicand1, matrix<T> multiplicand2) {
	matrix::hardCheckMatrices("ADD", multiplicand1, multiplicand2);
	matrix multiplier(multiplicand1.rows, multiplicand1.columns);
	for (size_t i = 0; i < multiplicand1.rows; i++) {
		for (size_t j = 0; j < multiplicand1.columns; j++) {
			multiplier[i][j] = (multiplicand1[i][j] + multiplicand2[i][j]);
		}
	}
	multiplier.update();
	return multiplier;
}

template <typename T>
matrix<T> matrix<T>::add(matrix<T> multiplicand1, T multiplicand2) {
	for (int i = 0; i < multiplicand1.rows; i++) {
		for (int j = 0; j < multiplicand1.columns; j++) {
			multiplicand1[i][j] += multiplicand2;
		}
	}
	return multiplicand1;
}

template <typename T>
void matrix<T>::sub(matrix<T> multiplicand) {
	hardCheck("SUB", multiplicand);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			base[i][j] -= multiplicand[i][j];
		}
	}
	update();
}

template <typename T>
matrix<T> matrix<T>::sub(matrix<T> multiplicand1, matrix<T> multiplicand2) {
	matrix::hardCheckMatrices("SUB", multiplicand1, multiplicand2);
	matrix multiplier(multiplicand1.rows, multiplicand1.columns);
	for (int i = 0; i < multiplicand1.rows; i++) {
		for (int j = 0; j < multiplicand1.columns; j++) {
			multiplier[i][j] = multiplicand1[i][j] - multiplicand2[i][j];
		}
	}
	multiplier.update();
	return multiplier;
}

template <typename T>
void matrix<T>::dot(matrix<T> multiplicand) {
	hardCheck("DOT", multiplicand);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			base[i][j] *= multiplicand[i][j];
		}
	}
	update();
}

template <typename T>
matrix<T> matrix<T>::dot(matrix<T> multiplicand1, matrix<T> multiplicand2) {
	matrix::hardCheckMatrices("DOT", multiplicand1, multiplicand2);
	matrix multiplier(multiplicand1.rows, multiplicand1.columns);
	for (size_t i = 0; i < multiplicand1.rows; i++) {
		for (size_t j = 0; j < multiplicand1.columns; j++) {
			multiplier[i][j] = multiplicand1[i][j] * multiplicand2[i][j];
		}
	}
	multiplier.update();
	return multiplier;
}

template <typename T>
void matrix<T>::cross(matrix<T> multiplicand) {
	hardCheck("CROSS", multiplicand);
	matrix multiplier(rows, multiplicand.columns);
	for (int i = 0; i < multiplier.rows; i++) {
		for (int j = 0; j < multiplier.columns; j++) {
			double k = 0;
			for (int l = 0; l < columns; l++) {
				k += base[i][l] * multiplicand[l][j];
			}
			multiplier[i][j] = k;
		}
	}
	base = multiplier.base;
	update();
}

template <typename T>
matrix<T> matrix<T>::cross(matrix<T> multiplicand1, matrix<T> multiplicand2) {
	matrix::hardCheckMatrices("CROSS", multiplicand1, multiplicand2);
	matrix multiplier(multiplicand1.rows, multiplicand2.columns);
	for (size_t i = 0; i < multiplier.rows; i++) {
		for (size_t j = 0; j < multiplier.columns; j++) {
			double k = 0;
			for (size_t l = 0; l < multiplicand1.columns; l++) {
				k += multiplicand1[i][l] * multiplicand2[l][j];
			}
			multiplier[i][j] = k;
		}
	}
	return multiplier;
}

template <typename T>
void matrix<T>::transpose() {
	matrix<double> result(columns, rows);
	for (size_t i = 0; i < rows; i++) {
		for (size_t j; j < columns; j++) {
			result[j][i] = base[i][j];
		}
	}
	base = result.base;
	update();
}

template <typename T>
matrix<T> matrix<T>::transpose(matrix<T> original) {
	matrix<double> result(original.getColumns(), original.getRows());
	for (size_t i = 0; i < original.getRows(); i++) {
		for (size_t j = 0; j < original.getColumns(); j++) {
			result[j][i] = original[i][j];
		}
	}
	return result;
}

template <typename T>
void matrix<T>::transform(std::function<T(T)> f) {
	for_each(base.begin(), base.end(), [&](std::vector<T>& vec) {for_each(vec.begin(), vec.end(), [&](T& i) { i = f(i); }); });
}

template <typename T>
matrix<T> matrix<T>::transform(matrix<T> original, std::function<T(T)> f) {
	for_each(original.base.begin(), original.base.end(), [&](std::vector<T>& vec) {for_each(vec.begin(), vec.end(), [&](T& i) { i = f(i); }); });
	return original;
}

template <typename T>
cv::Mat matrix<T>::visualize() {
	cv::Mat visualization(int(rows), int(columns), CV_64F);
	for (size_t i = 0; i < rows; i++) {
		for (size_t j = 0; j < columns; j++) {
			visualization.at<double>(int(i), int(j)) = base[i][j];
		}
	}
	return visualization;
}