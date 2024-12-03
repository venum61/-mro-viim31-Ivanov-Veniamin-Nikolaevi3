#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <cmath>
#include <algorithm>
#include <string>
#include <sstream>

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

// Функция для вычисления псевдообратной матрицы
std::vector<std::vector<double>> pseudoInverse(const std::vector<std::vector<double>>& matrix) {
    int rows = matrix.size();
    int cols = matrix[0].size();

    // Вычисление транспонированной матрицы
    std::vector<std::vector<double>> transposedMatrix(cols, std::vector<double>(rows));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            transposedMatrix[j][i] = matrix[i][j];
        }
    }

    // Вычисление произведения транспонированной на исходную матрицу
    std::vector<std::vector<double>> productMatrix(cols, std::vector<double>(cols, 0.0));
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < cols; j++) {
            for (int k = 0; k < rows; k++) {
                productMatrix[i][j] += transposedMatrix[i][k] * matrix[k][j];
            }
        }
    }

    // Вычисление обратной матрицы
    std::vector<std::vector<double>> inverseMatrix = inverse(productMatrix);

    // Вычисление псевдообратной матрицы
    std::vector<std::vector<double>> pseudoInverseMatrix(cols, std::vector<double>(rows, 0.0));
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            for (int k = 0; k < cols; k++) {
                pseudoInverseMatrix[i][j] += inverseMatrix[i][k] * transposedMatrix[k][j];
            }
        }
    }

    return pseudoInverseMatrix;
}

// Функция для вычисления обратной матрицы
std::vector<std::vector<double>> inverse(std::vector<std::vector<double>>& matrix) {
    int n = matrix.size();
    std::vector<std::vector<double>> inverseMatrix(n, std::vector<double>(n, 0.0));

    // Создание единичной матрицы
    for (int i = 0; i < n; i++) {
        inverseMatrix[i][i] = 1.0;
    }

    // Метод Гаусса-Жордана для вычисления обратной матрицы
    for (int i = 0; i < n; i++) {
        // Деление строки на ведущий элемент
        double pivot = matrix[i][i];
        for (int j = 0; j < n; j++) {
            matrix[i][j] /= pivot;
            inverseMatrix[i][j] /= pivot;
        }

        // Устранение других элементов в столбце
        for (int k = 0; k < n; k++) {
            if (k != i) {
                double factor = matrix[k][i];
                for (int j = 0; j < n; j++) {
                    matrix[k][j] -= factor * matrix[i][j];
                    inverseMatrix[k][j] -= factor * inverseMatrix[i][j];
                }
            }
        }
    }

    return inverseMatrix;
}

// Функция для умножения матрицы на вектор
std::vector<double> multiplyMatrixVector(const std::vector<std::vector<double>>& matrix, const std::vector<double>& vector) {
    int rows = matrix.size();
    int cols = matrix[0].size();
    std::vector<double> result(rows, 0.0);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
    }

    return result;
}

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

    // Вычисление псевдообратной матрицы
    std::vector<std::vector<double>> pseudoInverseMatrix = pseudoInverse(data);

    // Выбор произвольного вектора с положительными координатами
    std::vector<double> vector(data[0].size(), 0.0);
    for (int i = 0; i < vector.size(); i++) {
        vector[i] = rand() % 10 + 1; // Случайное значение от 1 до 10
    }

    // Цикл для поиска вектора, который даёт положительное произведение
    bool found = false;
    while (!found) {
        // Умножение вектора на псевдообратную матрицу
        std::vector<double> result = multiplyMatrixVector(pseudoInverseMatrix, vector);

        // Умножение результата на исходную матрицу
        double product = 0.0;
        for (int i = 0; i < data.size(); i++) {
            for (int j = 0; j < data[0].size(); j++) {
                product += data[i][j] * result[j];
            }
        }

        // Если произведение больше нуля, то алгоритм завершается
        if (product > 0) {
            found = true;
            std::cout << "Найден вектор, дающий положительное произведение" << std::endl;
            std::cout << "Вектор: ";
            for (double val : vector) {
                std::cout << val << " ";
            }
            std::cout << std::endl;
            std::cout << "Результат: ";
            for (double val : result) {
                std::cout << val << " ";
            }
            std::cout << std::endl;

            // Запись результата в файл
            std::ofstream outputFile("line.txt");
            if (outputFile.is_open()) {
                for (double val : result) {
                    outputFile << val << " ";
                }
                outputFile.close();
                std::wcout << L"Результат записан в файл result.txt" << std::endl;
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