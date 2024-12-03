#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <ctime>

using namespace std;

// Структура для хранения точки
struct Point {
    double x;
    double y;
    int class_num;
};

// Функция для вычисления евклидова расстояния
double euclidean_distance(const Point &a, const Point &b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

// Алгоритм Forel
void forel(const vector<Point> &data, int k) {
    int n = data.size();
    vector<Point> centroids(k);
    vector<int> labels(n, 0);

    // Инициализируем центры кластеров случайным образом
    srand(time(0));
    for (int i = 0; i < k; ++i) {
        centroids[i] = data[rand() % n];
    }

    bool converged = false;

    while (!converged) {
        // Присваиваем точки ближайшим центрам
        for (int i = 0; i < n; ++i) {
            double min_distance = euclidean_distance(data[i], centroids[0]);
            labels[i] = 0;

            for (int j = 1; j < k; ++j) {
                double distance = euclidean_distance(data[i], centroids[j]);
                if (distance < min_distance) {
                    min_distance = distance;
                    labels[i] = j;
                }
            }
        }

        // Обновляем центры кластеров
        vector<Point> new_centroids(k);
        vector<int> counts(k, 0);

        for (int i = 0; i < n; ++i) {
            new_centroids[labels[i]].x += data[i].x;
            new_centroids[labels[i]].y += data[i].y;
            counts[labels[i]]++;
        }

        for (int j = 0; j < k; ++j) {
            if (counts[j] > 0) {
                new_centroids[j].x /= counts[j];
                new_centroids[j].y /= counts[j];
            } else {
                new_centroids[j] = data[rand() % n]; // случайный центр, если кластер пуст
            }
        }

        // Проверяем сходимость
        converged = true;
        for (int j = 0; j < k; ++j) {
            if (euclidean_distance(centroids[j], new_centroids[j]) > 1e-6) {
                converged = false;
                break;
            }
        }

        centroids = new_centroids;
    }

    // Сохраняем результаты в файл
    ofstream outfile("result.txt");
    for (int i = 0; i < n; ++i) {
        outfile << data[i].x << " " << data[i].y << " " << labels[i] + 1 << endl; // +1 для соответствия с Python
    }
    outfile.close();

    // Сохраняем центры кластеров в файл
    ofstream centroid_outfile("centroids.txt");
    for (const auto &centroid : centroids) {
        centroid_outfile << centroid.x << " " << centroid.y << endl;
    }
    centroid_outfile.close();
}

int main() {
    // Считываем данные из файла
    vector<Point> data;
    ifstream infile("data.txt");
    
    double x, y;
    while (infile >> x >> y) {
        data.push_back({x, y, 0});
    }
    infile.close();

    int k = 3; // Количество кластеров
    forel(data, k);

    return 0;
}
