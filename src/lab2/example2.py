import numpy as np
X = np.array([[6, 9, 1], [5, 7, 1], [5, 9, 1], [0, 10, 1]])
y = np.array([[1], [1], [-1], [-1]])
w = np.dot(np.linalg.pinv(X), y)
np.dot(X, w)
Y = y * X
b = np.ones([4, 1])
b[3] = 10
w = np.dot(np.linalg.pinv(Y), b)
np.dot(Y, w)
e = -np.inf * np.ones([4, 1])
b = np.ones([4, 1])
while np.any(e < 0):
    w = np.dot(np.linalg.pinv(Y), b)
    e = b - np.dot(Y, w)
    b = b - e * (e < 0)
print(b)
print(w)