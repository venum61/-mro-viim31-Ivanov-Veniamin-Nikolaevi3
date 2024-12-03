import numpy as np
import matplotlib.pyplot as plt

def read_data(filename):
    labels = []
    coordinates_list = []
    max_dim = 0 # отслеживаем максимальное количество координат

    with open(filename, 'r') as f:
        for line in f:
            line = line.strip().split()
            label = int(line[0])
            coordinates = np.array([float(x) for x in line[1:]])
            labels.append(label)
            coordinates_list.append(coordinates)
            max_dim = max(max_dim, len(coordinates))

    # Создаем массив с нужной формой, заполняя недостающие значения NaN
    coordinates_array = np.full((len(labels), max_dim), np.nan)
    for i, coords in enumerate(coordinates_list):
        coordinates_array[i, :len(coords)] = coords

    return np.column_stack((np.array(labels), coordinates_array))


data = read_data("points.txt")
labels = data[:, 0].astype(int)
coordinates = data[:, 1:] # Изменение: Теперь берем все столбцы, кроме первого (меток)

dimension = coordinates.shape[1]

if dimension == 2:
    plt.figure(figsize=(8, 6))
    plt.scatter(coordinates[:, 0], coordinates[:, 1], c=labels, cmap='viridis')
    plt.xlabel("X")
    plt.ylabel("Y")
    plt.title("2D")
elif dimension == 3:
    fig = plt.figure(figsize=(10, 8))
    ax = fig.add_subplot(111, projection='3d')
    ax.scatter(coordinates[:, 0], coordinates[:, 1], coordinates[:, 2], c=labels, cmap='viridis')
    ax.set_xlabel("X")
    ax.set_ylabel("Y")
    ax.set_zlabel("Z")
    ax.set_title("3D Визуализация")
else:
    print(f"Визуализация не поддерживается для {dimension}-размерные данные.")
    exit()

plt.show()