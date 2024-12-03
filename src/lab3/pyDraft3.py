import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np

def parse_output_file(file_path):
    points = []
    centers = []
    distances = []
    section = 'points'  # Текущая секция для разбора

    with open(file_path, 'r', encoding='utf-8') as f:  # Указание кодировки UTF-8
        for line in f:
            line = line.strip()
            if line == "":
                continue

            if line.startswith("Центры классов"):
                section = 'centers'
                continue
            elif line.startswith("Расстояния между центрами классов"):
                section = 'distances'
                continue
            elif line.startswith("Расстояния между указанными точками"):
                section = 'query_distances'
                continue

            if section == 'points':
                points.append(list(map(float, line.split())))
            elif section == 'centers':
                centers.append(list(map(float, line.split())))
            elif section in ['distances', 'query_distances']:
                distances.append(line)

    return np.array(points), np.array(centers), distances

def visualize(points, centers, distances):
    # Проверим, сколько измерений у точек
    if points.ndim == 1:
        points = points.reshape(-1, 2)  # Если есть только 1D, сделать 2D

    if points.shape[1] == 2:
        plt.figure(figsize=(10, 6))
        plt.scatter(points[:, 0], points[:, 1], c='blue', label='Точки')
        plt.scatter(centers[:, 0], centers[:, 1], c='red', marker='X', s=200, label='Центры классов')

        for i, center in enumerate(centers):
            plt.annotate(f'Центр {i+1}', (center[0], center[1]), textcoords="offset points", xytext=(0,10), ha='center')

        plt.title('Визуализация 2D точек и центров классов')
        plt.xlabel('X')
        plt.ylabel('Y')
        plt.legend()
        plt.grid()
        plt.show()

    elif points.shape[1] == 3:
        fig = plt.figure(figsize=(10, 6))
        ax = fig.add_subplot(111, projection='3d')

        ax.scatter(points[:, 0], points[:, 1], points[:, 2], c='green', label='Точки')
        ax.scatter(centers[:, 0], centers[:, 1], centers[:, 2], c='black', marker='d', s=200, label='Центры классов')

        for i, center in enumerate(centers):
            ax.text(center[0], center[1], center[2], f'Центр {i+1}', color='blue')

        ax.set_title('Визуализация точек и центров классов')
        ax.set_xlabel('X')
        ax.set_ylabel('Y')
        ax.set_zlabel('Z')
        ax.legend()
        plt.show()

    else:
        print("Недостаточно измерений для визуализации.")

    print("\nРасстояния между центрами классов:")
    for distance in distances:
        print(distance)

if __name__ == "__main__":
    # Укажите путь к выходному файлу, генерируемому вашей программой на C++
    output_file_path = "points.txt"

    points, centers, distances = parse_output_file(output_file_path)
    visualize(points, centers, distances)