#ifndef OBJECTS_H
#define OBJECTS_H

#define MAX_DIMENSIONS 10
#define MAX_POINTS 100
#define MAX_LINES 100
#define OBJECT_PATH "res/obj/"
#define OBJECT_EXTENSION ".txt"
#define READ_POINTS_STATE 1
#define READ_LINES_STATE 2

class Point{
private:
    double position[MAX_DIMENSIONS];
public:
    Point();
    Point(double position[], unsigned dim);
    void setPosition(double position, unsigned axis);
    void setPosition(double position[], unsigned dim);
    double getPosition(unsigned axis);
};

class Line{
private:
    Point* points[2];
    float color[3];
public:
    Line();
    Line(Point* p1, Point* p2);
    void setPoints(Point* p1, Point* p2);
    void setPoint(Point* p, unsigned index);
    void setColor(float red, float green, float blue);
    void setColor(float value, unsigned index);
    float red();
    float green();
    float blue();
    Point getPoint(unsigned index);
};

class Object{
private:
    unsigned n_lines;
    unsigned n_points;
    double scale[MAX_DIMENSIONS];
    double position[MAX_DIMENSIONS];
    Line lines[MAX_LINES];
    Point points[MAX_POINTS];
    void load(std::string object);
public:
    Object(std::string object);
    Object(std::string object, double scale);
    void setScale(double scale);
    void setScale(double scale, unsigned axis);
    void setPosition(double position, unsigned axis);
    void stretch(double factor);
    void stretch(double factor, unsigned axis);
    void translate(double amount, unsigned axis);
    void rotate(double angle, unsigned axis1, unsigned axis2);
    void draw(double xcp, double ycp, double zcp, double zvp);
    double getScale(unsigned axis);
    double getPosition(unsigned axis);
};

#endif