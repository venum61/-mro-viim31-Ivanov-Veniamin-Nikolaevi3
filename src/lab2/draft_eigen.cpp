#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <cmath>
#include <algorithm>
#include <string>
#include <sstream>

#include <eigen3/Eigen> // Добавьте заголовок Eigen для матричных операций

class DataGenerator {
private:
    std::string inputFile;
    std::string outputFile;
    int numClasses;
    int numPointsPerClass;
    int dimension;
    std::vector<std::vector<double>> classRanges;

public:
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
            for (int i = 0; i < numClasses; i++) {
                for (int j = 0; j < numPointsPerClass; j++) {
                    // Генерация случайных координат x, y, z в заданном диапазоне
                    double x = classRanges[i][0] + static_cast <double> (rand()) /(static_cast <double> (RAND_MAX/(classRanges[i][1] - classRanges[i][0])));
                    double y = classRanges[i][2] + static_cast <double> (rand()) /(static_cast <double> (RAND_MAX/(classRanges[i][3] - classRanges[i][2])));
                    double z = classRanges[i][4] + static_cast <double> (rand()) /(static_cast <double> (RAND_MAX/(classRanges[i][5] - classRanges[i][4])));
                    // Запись сгенерированной точки в выходной файл с меткой класса
                    file << x << " " << y << " " << z << " " << (i == 0 ? 1 : -1) << std::endl;
                }
                file << "-" << std::endl;
            }
            file.close();
        }
    }
};

int main() {
    setlocale(LC_ALL, "ru");
    DataGenerator generator("config.txt", "points.txt");
    generator.readInputData();
    generator.generateOutputData();

    std::cout << "Данные точек записаны" << std::endl;

    // Чтение данных из файла
    std::ifstream file("points.txt");
    std::vector<std::vector<double>> data;
    std::vector<int> labels;
    std::string line;
    while (std::getline(file, line)) {
        if (line == "-") {
            continue;
        }
        std::vector<double> sample;
        int label;
        std::stringstream ss(line);
        double x, y, z;
        ss >> x >> y >> z >> label;
        sample.push_back(x);
        sample.push_back(y);
        sample.push_back(z);
        data.push_back(sample);
        labels.push_back(label);
    }
    file.close();

    // Создание матрицы данных
    Eigen::MatrixXd dataMatrix(data.size(), data[0].size());
    for (int i = 0; i < data.size(); i++) {
        for (int j = 0; j < data[0].size(); j++) {
            dataMatrix(i, j) = data[i][j];
        }
    }

    // Вычисление псевдообратной матрицы
    Eigen::MatrixXd pseudoInverse = dataMatrix.transpose() * dataMatrix;
    pseudoInverse = pseudoInverse.inverse() * dataMatrix.transpose();

    // Выбор произвольного вектора с положительными координатами
    Eigen::VectorXd vector(data[0].size());
    for (int i = 0; i < vector.size(); i++) {
        vector[i] = rand() % 10 + 1; // Случайное значение от 1 до 10
    }

    // Цикл для поиска вектора, который даёт положительное произведение
    bool found = false;
    while (!found) {
        // Умножение вектора на псевдообратную матрицу
        Eigen::VectorXd result = pseudoInverse * vector;

        // Умножение результата на исходную матрицу
        double product = (dataMatrix * result).dot(result);

        // Если произведение больше нуля, то алгоритм завершается
        if (product > 0) {
            found = true;
            std::cout << "Найден вектор, дающий положительное произведение" << std::endl;
            std::cout << "Вектор: " << vector.transpose() << std::endl;
            std::cout << "Результат: " << result.transpose() << std::endl;

            // Запись результата в файл
            std::ofstream outputFile("result.txt");
            if (outputFile.is_open()) {
                outputFile << result.transpose() << std::endl;
                outputFile.close();
                std::cout << "Результат записан в файл result.txt" << std::endl;
            } else {
                std::cerr << "Не удалось открыть файл result.txt" << std::endl;
            }
        } else {
            // Выбор нового вектора, если произведение меньше нуля
            for (int i = 0; i < vector.size(); i++) {
                vector[i] = rand() % 10 + 1;
            }
        }
    }

    return 0;
}