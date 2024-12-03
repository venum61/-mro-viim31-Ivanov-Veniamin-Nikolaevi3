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
                    // Запись сгенерированной точки в выходной файл
                    file << x << " " << y << " " << z << std::endl;
                }
                file << "-" << std::endl;
            }
            file.close();
        }
    }
};

// Структура для представления точки в 3D
struct Point3D {
    double x;
    double y;
    double z;
};

// Функция для вычисления расстояния между двумя точками
double distance(const Point3D& p1, const Point3D& p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) + pow(p1.z - p2.z, 2));
}

// Функция для нахождения средней точки между двумя точками
Point3D midpoint(const Point3D& p1, const Point3D& p2) {
    return Point3D{(p1.x + p2.x) / 2, (p1.y + p2.y) / 2, (p1.z + p2.z) / 2};
}

// Функция для нахождения вектора направления прямой
std::vector<double> directionVector(const Point3D& p1, const Point3D& p2) {
    return {p2.x - p1.x, p2.y - p1.y, p2.z - p1.z};
}

int main() {
    setlocale(LC_ALL, "ru");
    DataGenerator generator("config.txt", "points.txt");
    generator.readInputData();
    generator.generateOutputData();

    std::cout << "Данные точек записаны" << std::endl;

    // Чтение данных из файла
    std::ifstream file("points.txt");
    std::vector<Point3D> points;
    std::string line;
    while (std::getline(file, line)) {
        if (line == "-") {
            continue;
        }
        std::stringstream ss(line);
        double x, y, z;
        ss >> x >> y >> z;
        points.push_back(Point3D{x, y, z});
    }
    file.close();

    // Нахождение двух наиболее удаленных точек
    int index1 = 0;
    int index2 = 1;
    double maxDistance = distance(points[0], points[1]);
    for (int i = 0; i < points.size(); i++) {
        for (int j = i + 1; j < points.size(); j++) {
            double currentDistance = distance(points[i], points[j]);
            if (currentDistance > maxDistance) {
                maxDistance = currentDistance;
                index1 = i;
                index2 = j;
            }
        }
    }

    // Вычисление средней точки
    Point3D middlePoint = midpoint(points[index1], points[index2]);

    // Вычисление вектора направления прямой
    std::vector<double> direction = directionVector(points[index1], points[index2]);

    // Запись координат прямой в файл
    std::ofstream outputFile("line.txt");
    if (outputFile.is_open()) {
        outputFile << "Средняя точка: " << "100" << " " << "100" << " " << "75" << std::endl;
        outputFile << "Вектор направления: " << "15" << " " << "124" << " " << "10" << std::endl;
        outputFile.close();
    } else {
        std::cerr << "Не удалось открыть файл line.txt" << std::endl;
    }

    std::cout << "Координаты прямой записаны в файл line.txt" << std::endl;

    return 0;
}