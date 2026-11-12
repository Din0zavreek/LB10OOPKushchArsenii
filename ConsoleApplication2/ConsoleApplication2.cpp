#define NOMINMAX
#include <windows.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <memory>
#include <algorithm>

class Point {
public:
    double x, y;

    Point(double x = 0, double y = 0) : x(x), y(y) {}

    double distanceTo(const Point& other) const {
        return std::sqrt(std::pow(other.x - x, 2) + std::pow(other.y - y, 2));
    }
};

class Triangle {
protected:
    Point p1, p2, p3;

public:
    Triangle(const Point& p1, const Point& p2, const Point& p3)
        : p1(p1), p2(p2), p3(p3) {
    }

    virtual ~Triangle() = default;

    virtual double perimeter() const {
        return p1.distanceTo(p2) + p2.distanceTo(p3) + p3.distanceTo(p1);
    }

    virtual double area() const {
        double a = p1.distanceTo(p2);
        double b = p2.distanceTo(p3);
        double c = p3.distanceTo(p1);
        double s = perimeter() / 2.0;
        return std::sqrt(s * (s - a) * (s - b) * (s - c));
    }

    virtual void printInfo() const {
        std::cout << "Вершини трикутника: "
            << "A(" << p1.x << "; " << p1.y << "), "
            << "B(" << p2.x << "; " << p2.y << "), "
            << "C(" << p3.x << "; " << p3.y << ")\n";
    }

    double getMedian(int vertexIndex) const {
        double a = p1.distanceTo(p2);
        double b = p2.distanceTo(p3);
        double c = p3.distanceTo(p1);

        if (vertexIndex == 1) {
            return 0.5 * std::sqrt(2 * std::pow(c, 2) + 2 * std::pow(a, 2) - std::pow(b, 2));
        }
        else if (vertexIndex == 2) {
            return 0.5 * std::sqrt(2 * std::pow(a, 2) + 2 * std::pow(b, 2) - std::pow(c, 2));
        }
        else if (vertexIndex == 3) {
            return 0.5 * std::sqrt(2 * std::pow(b, 2) + 2 * std::pow(c, 2) - std::pow(a, 2));
        }
        return 0.0;
    }

    static Triangle createByMedians(double m1, double m2, double m3, const Point& origin = { 0, 0 }) {
        double a = (2.0 / 3.0) * std::sqrt(2 * std::pow(m2, 2) + 2 * std::pow(m3, 2) - std::pow(m1, 2));
        double b = (2.0 / 3.0) * std::sqrt(2 * std::pow(m1, 2) + 2 * std::pow(m3, 2) - std::pow(m2, 2));
        double c = (2.0 / 3.0) * std::sqrt(2 * std::pow(m1, 2) + 2 * std::pow(m2, 2) - std::pow(m3, 2));

        Point pt1 = origin;
        Point pt2 = { origin.x + c, origin.y };
        double cosA = (std::pow(b, 2) + std::pow(c, 2) - std::pow(a, 2)) / (2 * b * c);
        double sinA = std::sqrt(1.0 - std::pow(cosA, 2));
        Point pt3 = { origin.x + b * cosA, origin.y + b * sinA };

        return Triangle(pt1, pt2, pt3);
    }

    double getMidline(int v1, int v2) const {
        double sideLength = 0.0;
        if ((v1 == 1 && v2 == 2) || (v1 == 2 && v2 == 1)) sideLength = p1.distanceTo(p2);
        else if ((v1 == 2 && v2 == 3) || (v1 == 3 && v2 == 2)) sideLength = p2.distanceTo(p3);
        else if ((v1 == 1 && v2 == 3) || (v1 == 3 && v2 == 1)) sideLength = p3.distanceTo(p1);

        return sideLength / 2.0;
    }
};

class TriangleByVertices : public Triangle {
public:
    TriangleByVertices(const Point& p1, const Point& p2, const Point& p3)
        : Triangle(p1, p2, p3) {
    }

    void printInfo() const override {
        std::cout << "[За трьома вершинами] ";
        Triangle::printInfo();
    }
};

class TriangleBySides : public Triangle {
public:
    TriangleBySides(double a, double b, double c)
        : Triangle({ 0, 0 }, { c, 0 }, { 0, 0 }) {
        double cosA = (std::pow(b, 2) + std::pow(c, 2) - std::pow(a, 2)) / (2 * b * c);
        double sinA = std::sqrt(1.0 - std::pow(cosA, 2));
        p3 = { b * cosA, b * sinA };
    }

    void printInfo() const override {
        std::cout << "[За трьома сторонами] ";
        Triangle::printInfo();
    }
};

class RightTriangle : public Triangle {
public:
    RightTriangle(const Point& p1, const Point& p2, const Point& p3)
        : Triangle(p1, p2, p3) {
    }

    void printInfo() const override {
        std::cout << "[Прямокутний трикутник] ";
        Triangle::printInfo();
        std::cout << "  Радіус вписаного кола: " << getInscribedRadius() << "\n";
        std::cout << "  Радіус описаного кола: " << getCircumscribedRadius() << "\n";
    }

    double getInscribedRadius() const {
        double a = p1.distanceTo(p2);
        double b = p2.distanceTo(p3);
        double c = p3.distanceTo(p1);
        double hyp = (std::max)({ a, b, c });
        double cat1 = (std::min)({ a, b, c });
        double cat2 = a + b + c - hyp - cat1;
        return (cat1 + cat2 - hyp) / 2.0;
    }

    double getCircumscribedRadius() const {
        double a = p1.distanceTo(p2);
        double b = p2.distanceTo(p3);
        double c = p3.distanceTo(p1);
        return (std::max)({ a, b, c }) / 2.0;
    }
};

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    std::cout << "=== ЛАБОРАТОРНА РОБОТА 10: ПОЛІМОРФІЗМ ===\n\n";

    std::vector<std::unique_ptr<Triangle>> triangles;

    triangles.push_back(std::make_unique<TriangleByVertices>(
        Point(0, 0), Point(4, 0), Point(0, 3)
    ));

    triangles.push_back(std::make_unique<TriangleBySides>(3.0, 4.0, 5.0));

    triangles.push_back(std::make_unique<RightTriangle>(
        Point(0, 0), Point(6, 0), Point(0, 8)
    ));

    int index = 1;
    for (const auto& t : triangles) {
        std::cout << "--- Трикутник #" << index++ << " ---\n";
        t->printInfo();
        std::cout << "Периметр: " << t->perimeter() << "\n";
        std::cout << "Площа: " << t->area() << "\n";
        std::cout << "Медіана до 1-ї вершини: " << t->getMedian(1) << "\n";
        std::cout << "Середня лінія (паралельна стороні 1-2): " << t->getMidline(1, 2) << "\n\n";
    }

    std::cout << "--- Створення трикутника за медіанами (3, 4, 5) ---\n";
    Triangle tMedians = Triangle::createByMedians(3.0, 4.0, 5.0);
    tMedians.printInfo();
    std::cout << "Площа створеного за медіанами трикутника: " << tMedians.area() << "\n";

    return 0;
}