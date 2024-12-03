#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <random>
#include <limits>
#include <algorithm>

class Point {
public:
    std::vector<double> coordinates;
    int label;

    Point(const std::vector<double>& coordinates, int label) : coordinates(coordinates), label(label) {}
};

class DataGenerator {
public:
    std::string inputFile;
    std::string outputFile;
    int numClasses;
    int numPointsPerClass;
    int dimension;
    std::vector<std::vector<double>> classRanges;

    DataGenerator(const std::string& input, const std::string& output) :
        inputFile(input), outputFile(output) {}

    void readInputData() {
        std::ifstream file(inputFile);
        if (file.is_open()) {
            file >> numClasses;
            file >> numPointsPerClass;
            file >> dimension;

            classRanges.resize(numClasses);
            for (int i = 0; i < numClasses; i++) {
                for (int j = 0; j < 6; j++) {
                    double value;
                    file >> value;
                    classRanges[i].push_back(value);
                }
            }

            file.close();
        } else {
            std::cerr << "Unable to open file: " << inputFile << std::endl;
        }
    }

    void generateOutputData() {
        std::ofstream file(outputFile);
        if (file.is_open()) {
            for (int i = 0; i < numClasses; ++i) {
                for (int j = 0; j < numPointsPerClass; ++j) {
                    std::vector<double> coordinates(dimension);
                    for (int k = 0; k < dimension; ++k) {
                        std::uniform_real_distribution<> dist(classRanges[i][k * 2], classRanges[i][k * 2 + 1]);
                        std::random_device rd;
                        std::mt19937 gen(rd());
                        coordinates[k] = dist(gen);
                    }
                    file << i << " "; // Записываем метку класса
                    for (double coord : coordinates) {
                        file << coord << " ";
                    }
                    file << std::endl;
                }
            }
            file.close();
        } else {
            std::cerr << "Unable to open file: " << outputFile << std::endl;
        }
    }
};

class Perceptron {
public:
    std::vector<double> weights;
    double bias;

    Perceptron(int dimensions) : weights(dimensions, 0.0), bias(0.0) {}

    double activation(double sum) const {
        return sum >= 0 ? 1 : 0; // Ступенчатая функция активации
    }

    int predict(const std::vector<double>& inputs) const {
        double sum = bias;
        for (size_t i = 0; i < weights.size(); ++i) {
            sum += weights[i] * inputs[i];
        }
        return activation(sum);
    }

    void train(const std::vector<Point>& points, int epochs, double learningRate) {
        for (int epoch = 0; epoch < epochs; ++epoch) {
            for (const auto& point : points) {
                int prediction = predict(point.coordinates);
                int error = point.label - prediction;
                bias += learningRate * error;
                for (size_t i = 0; i < weights.size(); ++i) {
                    weights[i] += learningRate * error * point.coordinates[i];
                }
            }
        }
    }
};


std::vector<Point> readData(const std::string& filename, int dimension) {
    std::vector<Point> points;
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::vector<double> coordinates(dimension);
            int label;
            ss >> label;
            for (int i = 0; i < dimension; ++i) {
                ss >> coordinates[i];
            }
            points.emplace_back(coordinates, label);
        }
        file.close();
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
    return points;
}
double accuracy(const std::vector<Point>& points, const Perceptron& perceptron) {
    int correct = 0;
    for (const auto& point : points) {
        if (perceptron.predict(point.coordinates) == point.label) {
            correct++;
        }
    }
    return static_cast<double>(correct) / points.size();
}

int main() {
    setlocale(LC_ALL, "ru");
    DataGenerator generator("config.txt", "points.txt");
    generator.readInputData();
    generator.generateOutputData();
    std::vector<Point> points = readData("points.txt", generator.dimension);
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(points.begin(), points.end(), g);
    size_t trainSize = points.size() * 0.8;
    std::vector<Point> trainSet(points.begin(), points.begin() + trainSize);
    std::vector<Point> testSet(points.begin() + trainSize, points.end());

    Perceptron perceptron(generator.dimension);
    perceptron.train(trainSet, 1000, 0.1);

    double acc = accuracy(testSet, perceptron);
    std::wcout << L"Точность на тестовом наборе: " << acc * 100 << "%" << std::endl;

    //Более детальный вывод для проверки
    for (const auto& point : testSet) {
        int prediction = perceptron.predict(point.coordinates);
        std::wcout << L"Фактический: " << point.label << L" Предсказанный: " << prediction << std::endl;
    }

    return 0;
}