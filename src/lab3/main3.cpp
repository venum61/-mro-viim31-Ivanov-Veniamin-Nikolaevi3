#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <cmath>
#include <utility>

// Определение класса Point
class Point {
public:
    explicit Point(int dimensions) : coordinates(dimensions) {}

    void setCoordinate(int index, double value) {
        coordinates[index] = value;
    }

    double getCoordinate(int index) const {
        return coordinates[index];
    }

    const std::vector<double>& getCoordinates() const {
        return coordinates;
    }

    int getDimensions() const {
        return coordinates.size();
    }

private:
    std::vector<double> coordinates;
};

// Определение класса Space
class Space {
public:
    Space(int pointCount, int dimensions, double minX, double maxX, double minY, double maxY, double minZ = 0.0, double maxZ = 0.0)
        : minX(minX), maxX(maxX), minY(minY), maxY(maxY), minZ(minZ), maxZ(maxZ) {
        points.reserve(pointCount);
        for (int i = 0; i < pointCount; ++i) {
            points.emplace_back(dimensions);
        }
    }

    void generatePoints() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> distX(minX, maxX);
        std::uniform_real_distribution<> distY(minY, maxY);
        std::uniform_real_distribution<> distZ(minZ, maxZ);

        for (auto& point : points) {
            point.setCoordinate(0, distX(gen));
            point.setCoordinate(1, distY(gen));
            if (point.getDimensions() == 3) {
                point.setCoordinate(2, distZ(gen));
            }
        }
    }

    const Point& getPoint(int index) const {
        return points[index];
    }

    int getPointCount() const {
        return points.size();
    }

    Point calculateCenter() const {
        int dimensions = points[0].getDimensions();
        Point center(dimensions);
        for (const auto& point : points) {
            for (int i = 0; i < dimensions; ++i) {
                center.setCoordinate(i, center.getCoordinate(i) + point.getCoordinate(i));
            }
        }
        for (int i = 0; i < dimensions; ++i) {
            center.setCoordinate(i, center.getCoordinate(i) / points.size());
        }
        return center;
    }

    static double calculateDistance(const Point& p1, const Point& p2) {
        double distance = 0.0;
        int dimensions = p1.getDimensions();
        for (int i = 0; i < dimensions; ++i) {
            distance += std::pow(p1.getCoordinate(i) - p2.getCoordinate(i), 2);
        }
        return std::sqrt(distance);
    }

    static double calculateManhattanDistance(const Point& p1, const Point& p2) {
        double distance = 0.0;
        int dimensions = p1.getDimensions();
        for (int i = 0; i < dimensions; ++i) {
            distance += std::abs(p1.getCoordinate(i) - p2.getCoordinate(i));
        }
        return distance;
    }

private:
    std::vector<Point> points;
    double minX, maxX, minY, maxY, minZ, maxZ;
};

int main() {
    std::ifstream inputFile("config.txt");
    if (!inputFile) {
        std::cerr << "Не удалось открыть файл" << std::endl;
        return 1;
    }

    int classCount;
    inputFile >> classCount;

    std::vector<Space> spaces;
    for (int i = 0; i < classCount; ++i) {
        int pointCount, dimensions;
        inputFile >> pointCount >> dimensions;
        double minX, maxX, minY, maxY, minZ = 0.0, maxZ = 0.0;
        inputFile >> minX >> maxX >> minY >> maxY >> minZ >> maxZ;

        spaces.emplace_back(pointCount, dimensions, minX, maxX, minY, maxY, minZ, maxZ);
    }

    // Генерация и запись координат точек
    std::ofstream outputFile("points.txt");
    if (!outputFile) {
        std::cerr << "Не удалось открыть файл" << std::endl;
        return 1;
    }

    for (auto& space : spaces) {
        space.generatePoints();
        for (int i = 0; i < space.getPointCount(); ++i) {
            const Point& point = space.getPoint(i);
            const auto& coordinates = point.getCoordinates();
            for (size_t j = 0; j < coordinates.size(); ++j) {outputFile << coordinates[j];
                if (j < coordinates.size() - 1) {
                    outputFile << " ";
                }
            }
            outputFile << '\n';
        }
    }

    // Нахождение центров классов и расстояний между ними
    std::vector<Point> centers;
    for (const auto& space : spaces) {
        centers.push_back(space.calculateCenter());
    }

    // Вывод центров классов
    outputFile << "\nЦентры классов:\n";
    for (const auto& center : centers) {
        for (int j = 0; j < center.getDimensions(); ++j) {
            outputFile << center.getCoordinate(j);
            if (j < center.getDimensions() - 1) {
                outputFile << " ";
            }
        }
        outputFile << '\n';
    }

    // Нахождение расстояний между центрами классов (евклидово расстояние)
    outputFile << "\nРасстояния между центрами классов (евклидово расстояние):\n";
    for (size_t i = 0; i < centers.size(); ++i) {
        for (size_t j = i + 1; j < centers.size(); ++j) {
            double distance = Space::calculateDistance(centers[i], centers[j]);
            outputFile << "Расстояние между классами " << (i + 1) << " и " << (j + 1) << ": " << distance << '\n';
        }
    }

    // Нахождение расстояний между центрами классов (манхэттенское расстояние)
    outputFile << "\nРасстояния между центрами классов (манхэттенское расстояние):\n";
    for (size_t i = 0; i < centers.size(); ++i) {
        for (size_t j = i + 1; j < centers.size(); ++j) {
            double distance = Space::calculateManhattanDistance(centers[i], centers[j]);
            outputFile << "Расстояние между классами " << (i + 1) << " и " << (j + 1) << ": " << distance << '\n';
        }
    }

    // Вычисление и вывод расстояний между указанными точками
    outputFile << "\nРасстояния между указанными точками:\n";
    std::vector<std::pair<int, int>> specificDistances;
    int queryCount;
    inputFile >> queryCount;
    for (int i = 0; i < queryCount; ++i) {
        int classIndex1, pointIndex1, classIndex2, pointIndex2;
        inputFile >> classIndex1 >> pointIndex1 >> classIndex2 >> pointIndex2;
        classIndex1 -= 1; // Приводим к 0-индексации
        pointIndex1 -= 1; // Приводим к 0-индексации
        classIndex2 -= 1; // Приводим к 0-индексации
        pointIndex2 -= 1; // Приводим к 0-индексации

        if (classIndex1 < spaces.size() && pointIndex1 < spaces[classIndex1].getPointCount() &&
            classIndex2 < spaces.size() && pointIndex2 < spaces[classIndex2].getPointCount()) {
            double distanceEuclidean = Space::calculateDistance(spaces[classIndex1].getPoint(pointIndex1),
                                                                spaces[classIndex2].getPoint(pointIndex2));
            // Вычисляем манхэттенское расстояние
            double distanceManhattan = Space::calculateManhattanDistance(spaces[classIndex1].getPoint(pointIndex1),
                                                                          spaces[classIndex2].getPoint(pointIndex2));
            outputFile << "Расстояние между точкой " << (pointIndex1 + 1) << " из класса " << (classIndex1 + 1)  
                       << " и точкой " << (pointIndex2 + 1) << " из класса " << (classIndex2 + 1) << ": ЕВКЛИДОВОЕ: " 
                       << distanceEuclidean << ", МАНХЭТТЕНСКОЕ: " << distanceManhattan << '\n';
        } 
    }

    outputFile.close();
    return 0;
}