import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

file_path = 'points.txt'

# Чтение строк из файла и разделение на координаты
points = []
with open(file_path, 'r') as file:
    for line in file:
        if line.startswith("-"):
            continue
        coords = line.strip().split()
        x, y, z = map(float, coords)
        points.append((x, y, z))

# Визуализация точек в трехмерном пространстве
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
for point in points:
    if point[0] == '-':
        ax.scatter(point[1], point[2], point[3], color='blue')  # Точки, начинающиеся с "-", будут синими
    else:
        ax.scatter(point[0], point[1], point[2], color='red')  # Обычные точки будут красными

ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
ax.set_title('Классы визуализированные на графике')

plt.show()