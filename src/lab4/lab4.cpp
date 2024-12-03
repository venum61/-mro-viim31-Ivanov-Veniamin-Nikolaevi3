#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include <limits>
#include<sstream>

// Class for a point
class Point {
public:
    Point(int dimensions) : coordinates(dimensions) {}

    void setCoordinate(int index, double value) { coordinates[index] = value; }
    double getCoordinate(int index) const { return coordinates[index]; }
    int getDimensions() const { return coordinates.size(); }

    double distance(const Point& other) const {
        double dist = 0.0;
        for (size_t i = 0; i < coordinates.size(); ++i) {
            dist += pow(coordinates[i] - other.coordinates[i], 2);
        }
        return sqrt(dist);
    }

    // Added tostring for debugging
    std::string toString() const {
      std::string s = "";
      for (double coord : coordinates) {
        s += std::to_string(coord) + " ";
      }
      return s;
    }

private:
    std::vector<double> coordinates;
};

// Class to manage a cluster (Space)
class Cluster {
public:
    Cluster(int dimensions) : center(dimensions) {}

    void addPoint(const Point& point) {
        points.push_back(point);
        recalculateCenter();
    }

    Point getCenter() const { return center; }
    int getPointCount() const { return points.size(); }

    // Calculate the center of the cluster
    void recalculateCenter() {
        if (points.empty()) return; // Avoid division by zero
        center.setCoordinate(0,0.0); // initialize
        for (const auto& point : points) {
            for (int i = 0; i < center.getDimensions(); ++i) {
              center.setCoordinate(i, center.getCoordinate(i) + point.getCoordinate(i));
            }
        }
        for (int i = 0; i < center.getDimensions(); ++i) {
            center.setCoordinate(i, center.getCoordinate(i) / points.size());
        }
    }


private:
    Point center;
    std::vector<Point> points;
};


// Class to manage spaces (clusters)
class SpaceManager {
public:
  SpaceManager(const std::vector<std::pair<int, int>>& spaceData) : spaceData(spaceData) {}

  // ... (your initialization code using the above classes)
  // ... (method to create Clusters)



private:
    std::vector<std::pair<int, int>> spaceData;
};


int main() {
    //Example input (replace with your input reading logic):
    std::vector<std::pair<int, int>> spaceData = {{10, 3}, {5, 3}};

    SpaceManager spaceManager(spaceData);
    std::vector<Cluster> clusters;

    // Example instantiation of clusters:
    for(const auto& space : spaceManager.spaceData){
        Cluster c(space.second);
        clusters.push_back(c);
    }


    //Example usage of the Cluster class
    Point p1(3);
    p1.setCoordinate(0, 1.0);
    p1.setCoordinate(1, 2.0);
    p1.setCoordinate(2, 3.0);

    clusters[0].addPoint(p1);

    Point p2(3);
    p2.setCoordinate(0, 4.0);
    p2.setCoordinate(1, 5.0);
    p2.setCoordinate(2, 6.0);

    clusters[0].addPoint(p2);
    // ... (rest of your main function)


    return 0;
}