import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np

file_path = 'points.txt'
line_file_path = 'line.txt'

# Чтение строк из файла points.txt и разделение на координаты
points = []
with open(file_path, 'r') as file:
    for line in file:
        if line.startswith("-"):
            continue
        coords = line.strip().split()
        x, y, z = map(float, coords)
        points.append((x, y, z))

# Чтение координат прямой из файла line.txt
center = [100, 100, 75]
direction_vector = [15, 124, 10]
with open(line_file_path, 'r') as file:
    for line in file:
        if "Средняя точка" in line:
            coords = line.split(":")[1].strip().split()
            if len(coords) == 3: # Проверяем, что координаты присутствуют
                center = [float(coord) for coord in coords]
        elif "Вектор направления" in line:
            coords = line.split(":")[1].strip().split()
            if len(coords) == 3: # Проверяем, что координаты присутствуют
                direction_vector = [float(coord) for coord in coords]

# Проверяем, что данные о прямой были найдены
if center is not None and direction_vector is not None:
    # Создание линии (отрезка прямой)
    t = np.linspace(-5, 5, 50)
    x = center[0] + t * direction_vector[0]
    y = center[1] + t * direction_vector[1]
    z = center[2] + t * direction_vector[2]

    # Визуализация точек и линии в трехмерном пространстве
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')

    # Визуализация точек
    for point in points:
        ax.scatter(point[0], point[1], point[2], color='red')

    # Визуализация линии
    ax.plot(x, y, z, color='blue')

    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    ax.set_title('Классы визуализированные на графике')

    plt.show()
else:
    print("Ошибка: Не удалось найти координаты прямой в файле line.txt")