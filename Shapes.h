#ifndef SHAPES_H
#define SHAPES_H
#include "Container.h"
#include "Base.h"

#include <iostream>

class Shape:public Printable {
    static size_t count;
public:
    Shape() {
        ++count;
    }
    ~Shape() {
        --count;
    }


    Shape& operator=(Shape const& shape) = delete;
    Shape& operator=(Shape && shape) = delete;
    Shape(Shape && shape) = default;

    Shape(Shape const&) { ++count; }
    static int GetCount() {
        return count;
    }
};


class Point:public Shape {
public:
    Point():x(0),y(0) {}
    Point(double x, double y):x(x),y(y) {}
    std::string to_string() const override {
        return "Point(" + std::to_string(x) + "," + std::to_string(y) + ");";
    }
    double x,y;
};

class Circle:public Shape {
public:
    Circle(double r = 0):radius(r) {}
    Circle(double x, double y, double r = 0):center(x,y),radius(r) {
        if(r < 0)
            throw std::invalid_argument("Invalid radius :" + std::to_string(r));
    }
    std::string to_string() const override {
        return "Circle((" + std::to_string(center.x) + "," + std::to_string(center.y) + "),"
                + std::to_string(radius) + ");";
    }
    Point center;
    double radius;
};


class Rect:public Shape {
public:
    Rect() {}
    Rect(double x1, double y1, double x2, double y2):p1(x1,y1),p2(x2,y2) {
        if(!(x1 <= x2 && y1 <= y2))
            throw std::invalid_argument("Can't create Rect from :"
                                        +p1.to_string() +"\n"
                                        +p2.to_string()
                                        );
    }
    std::string to_string() const override {
        return "Rect(\n("
                 + std::to_string(p1.x) + "," + std::to_string(p1.y)
                 + "),(" + std::to_string(p2.x) + "," + std::to_string(p1.y)
        + ")\n("
                + std::to_string(p1.x) + "," + std::to_string(p2.y)
                + "),(" + std::to_string(p2.x)
                + "," + std::to_string(p2.y)
        + ")\n);";
    }
    Point p1,p2; //(p.x,p.y)    (p.x+r.x,p.y)
               //(p.x,p.y+r.y)  (p.x+r.x,p.y+r.y)
};

class Square:public Shape {
public:
    Square():h(0) {}
    Square(double x, double y, double h):p(x,y),h(h) {}
    std::string to_string() const override {
        return  "Square(\n("
                + std::to_string(p.x) + "," + std::to_string(p.y)
                + "),(" + std::to_string(p.x+h) + "," + std::to_string(p.y)
       + ")\n("
               + std::to_string(p.x) + "," + std::to_string(p.y+h)
               + "),(" + std::to_string(p.x+h)
               + "," + std::to_string(p.y+h)
       + ")\n);";
    }
    Point p; //(p.x,p.y)    (p.x+h,p.y)
             //(p.x,p.y+h)  (p.x+h,p.y+h)
    double h;
};

class PolyLine:public Shape {
public:
    PolyLine() {}
    void AddPoint(Point const & point) {
        points.push_back(point);
    }
    std::string to_string() const override {
        return "PolyLine(\n" + get_points() + ");";
    }
    std::string get_points() const {
        std::string ret="";
        points.iterate([&ret](Point const&p) {
            ret += "(" + std::to_string(p.x) + "," + std::to_string(p.y) + ")\n";
            return true;
        });
        return ret;
    }
private:
    Container< Point > points;
};


class Polygon:public PolyLine {
public:
    std::string to_string() const override {
        return "Polygon(\n" + get_points() + ");";
    }
};




#endif // SHAPES_H
