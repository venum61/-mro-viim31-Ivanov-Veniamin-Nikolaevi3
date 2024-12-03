#include <iostream>
#include <fstream>
#include <vector>
#include <random>

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

     // Генерация точек для каждого класса
    double x_center, y_center, z_center;
    for (int i = 0; i < numClasses; i++) {
         for (int j = 0; j < numPointsPerClass; j++) {
             // Генерация случайных координат x, y, z в заданном диапазоне
             double x = classRanges[i][0] + static_cast <double> (rand()) /(static_cast <double> (RAND_MAX/(classRanges[i][1] - classRanges[i][0])));
             double y = classRanges[i][2] + static_cast <double> (rand()) /(static_cast <double> (RAND_MAX/(classRanges[i][3] - classRanges[i][2])));
             double z = classRanges[i][4] + static_cast <double> (rand()) /(static_cast <double> (RAND_MAX/(classRanges[i][5] - classRanges[i][4])));
            x_center = x / classRanges[i].size();
            y_center = y / classRanges[i].size();
            z_center = z / classRanges[i].size();
             // Запись сгенерированной точки в выходной файл
            file << x << " " << y << " " << z << std::endl;
         }
         file <<"- " << x_center << " " << y_center << " " << z_center << std::endl;
            }
        }
         file.close();
    }
 };

int main() {
    setlocale(LC_ALL, "ru");
    DataGenerator generator("config.txt", "points.txt");
    generator.readInputData();
    generator.generateOutputData();
    std::wcout << L"Данные точек и центры классов записаны"<<std::endl;
    return 0;
}
