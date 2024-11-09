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
    void setPosition(double position[], unsigned dim);
    void setPosition(unsigned index, double value);
    void rotate(double angle, unsigned axis1, unsigned axis2);
    double getPosition(unsigned index);
};

class Line{
private:
    Point* points[2];
    float color[3];
public:
    Line();
    Line(Point* p1, Point* p2);
    void setPoints(Point* p1, Point* p2);
    void setPoint(unsigned index, Point* p);
    void setColor(float red, float green, float blue);
    void setColor(unsigned index, float value);
    void rotate(double angle, unsigned axis1, unsigned axis2);
    float red();
    float green();
    float blue();
    Point getPoint(unsigned index);
};

class Object{
private:
    unsigned n_points;
    unsigned n_lines;
    double position[MAX_DIMENSIONS];
    double scale[MAX_DIMENSIONS];
    Point points[MAX_POINTS];
    Line lines[MAX_LINES];
    void load(std::string object);
public:
    Object(std::string object);
    Object(std::string object, double scale);
    void setScale(double scale);
    void rotate(double angle, unsigned axis1, unsigned axis2);
    double getScale(unsigned index);
    unsigned linesCount();
    Line getLine(unsigned index);
};

// class Tesseract{
// private:
//     Point points[16];
//     Line lines[32];

//     void init(double size);
//     void updateLines();
// public:
//     Tesseract();
//     Tesseract(double size);
//     const int linesCount();
//     Line getLine(unsigned index);
//     void rotate(double angle, unsigned axis1, unsigned axis2);
// };

#endif