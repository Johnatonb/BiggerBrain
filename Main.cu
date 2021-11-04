#include <fstream>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "BigBrain.hpp"
#include <stdio.h>

matrix<double> readImagesColumn(std::ifstream& file, int imageNum) {
    matrix<double> image(784, 1);
    file.seekg(16 + (784 * imageNum));
    for (size_t j = 0; j < 784; j++) {
        unsigned char temp;
        file.read((char*)&temp, sizeof(temp));
        image[j][0] = (double)temp / 255;
    }
    return image;
}


matrix<double> readImagesSquare(std::ifstream& file, int imageNum) {
    matrix<double> image(28, 28);
    file.seekg(16 + (784 * imageNum));
    for (size_t i = 0; i < 28; i++) {
        for (size_t j = 0; j < 28; j++) {
            unsigned char temp;
            file.read((char*)&temp, sizeof(temp));
            image[i][j] = (double)temp / 255;
        }
    }
    return image;
}

matrix<double> readLabel(std::ifstream& file, int imageNum) {
    matrix<double> label(10, 1);
    file.seekg(8 + imageNum);
    unsigned char temp;
    file.read((char*)&temp, sizeof(temp));
    //std::cout << (double)temp << std::endl;
    label[temp][0] = double(1);
    return label;
}


int main(){
	std::string trainingImages = R"(C:\Users\johnk\Downloads\train-images-idx3-ubyte\train-images.idx3-ubyte)";
	std::ifstream imageFile(trainingImages.c_str(), std::ios::binary);
	std::string trainingLabels = R"(C:\Users\johnk\Downloads\train-labels-idx1-ubyte\train-labels.idx1-ubyte)";
	std::ifstream labelFile(trainingLabels.c_str(), std::ios::binary);

	matrix<double> image = readImagesSquare(imageFile, 0);

	std::vector<matrix<double>> images;
	std::vector<matrix<double>> labels;

	for (size_t i = 0; i < 1000; i++) {
		images.push_back(readImagesColumn(imageFile, i));
		labels.push_back(readLabel(labelFile, i));
	}

	std::cout << "Finished Reading" << std::endl;

	bigBrain network(std::vector<size_t>{ 784, 16, 16, 10 });
	network.initialize();
	/*for (int i = 0; i < 100; i++) {
		network.learn(images, labels);
		std::cout << i << "%" << std::endl;
	}

	matrix<double> output = network.think(images[0]);
	for (int i = 0; i < 10; i++) {
		std::cout << output[i][0] << std::endl;
	}

	cv::imshow("my image", image.visualize());
	cv::waitKey(0);

	/*for (size_t i = 1; i <= 28; i++) {
		for (size_t j = 1; j <= 28; j++) {
			if (ceil(image[(i * j) - size_t(1)][0]) == 1) { std::cout << "."; }
			else { std::cout << " "; }
		}
		std::cout<<std::endl;
	}*/
}