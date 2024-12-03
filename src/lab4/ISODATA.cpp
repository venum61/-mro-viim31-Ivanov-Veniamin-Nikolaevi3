#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>

void isodata(const std::vector<std::pair<double, double>> &points, int initial_k, const std::string &filename, int max_iter = 100, double tol = 1e-4)
{
    int n_points = points.size();
    std::vector<std::pair<double, double>> centers(initial_k);
    std::vector<int> labels(n_points);

    // Инициализация центров кластеров случайными точками
    std::srand(42); // Для воспроизводимости
    for (int i = 0; i < initial_k; ++i)
    {
        int random_index = std::rand() % n_points;
        centers[i] = points[random_index];
    }

    for (int iteration = 0; iteration < max_iter; ++iteration)
    {
        // Шаг 1: Присвоение меток кластерам
        std::vector<std::vector<double>> distances(n_points, std::vector<double>(initial_k));
        for (int i = 0; i < initial_k; ++i)
        {
            for (int j = 0; j < n_points; ++j)
            {
                distances[j][i] = std::sqrt(std::pow(points[j].first - centers[i].first, 2) + std::pow(points[j].second - centers[i].second, 2));
            }
        }

        for (int j = 0; j < n_points; ++j)
        {
            labels[j] = std::min_element(distances[j].begin(), distances[j].end()) - distances[j].begin();
        }

        // Шаг 2: Обновление центров кластеров
        std::vector<std::pair<double, double>> new_centers(initial_k, {0.0, 0.0});
        std::vector<int> counts(initial_k, 0);

        for (int j = 0; j < n_points; ++j)
        {
            new_centers[labels[j]].first += points[j].first;
            new_centers[labels[j]].second += points[j].second;
            counts[labels[j]]++;
        }

        for (int i = 0; i < initial_k; ++i)
        {
            if (counts[i] > 0)
            {
                new_centers[i].first /= counts[i];
                new_centers[i].second /= counts[i];
            }
            else
            {
                new_centers[i] = centers[i]; // Если кластер пустой, оставляем старый центр
            }
        }

        // Проверка на сходимость
        double max_shift = 0.0;
        for (int i = 0; i < initial_k; ++i)
        {
            max_shift = std::max(max_shift, std::sqrt(std::pow(new_centers[i].first - centers[i].first, 2) + std::pow(new_centers[i].second - centers[i].second, 2)));
        }

        if (max_shift < tol)
        {
            break;
        }

        centers = new_centers;
    }

    // Сохранение результатов в файл
    std::ofstream outfile(filename);
    for (int j = 0; j < n_points; ++j)
    {
        outfile << points[j].first << " " << points[j].second << " " << labels[j] + 1 << std::endl;
    }

    // Сохранение центров кластеров
    for (const auto &center : centers)
    {
        outfile << center.first << " " << center.second << " " << 4 << std::endl;
    }
}

int main()
{
    std::ifstream infile("data.txt");
    std::vector<std::pair<double, double>> points;

    double x, y;
    int class_label;

    while (infile >> x >> y >> class_label)
    {
        points.emplace_back(x, y);
    }

    int initial_k = 3;
    isodata(points, initial_k, "result.txt");
    return 0;
}
