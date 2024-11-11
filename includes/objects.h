#ifndef OBJECTS_H
#define OBJECTS_H

#define MAX_RENDERABLES 100
#define MAX_DIMENSIONS 10
#define MAX_POINTS 100
#define MAX_LINES 99
#define MAX_FACES 66

#define OBJECT_PATH "res/obj/"
#define OBJECT_EXTENSION ".txt"

#define READ_POINTS_STATE 1
#define READ_LINES_STATE 2
#define READ_FACES_STATE 3

#define LINE_N_POINTS 2
#define FACE_N_POINTS 3

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

class Renderable{
protected:
    unsigned nPoints;
    float color[3];
    Point* points[3];
public:
    void setColor(float red, float green, float blue);
    void setColor(float value, unsigned index);
    void setPoint(Point* p, unsigned index);
    unsigned getNPoints();
    float red();
    float green();
    float blue();
    Point getPoint(unsigned index);
};

class Line : public Renderable {
public:
    Line();
    Line(Point* p1, Point* p2);
    void setPoints(Point* p1, Point* p2);
};

class Face : public Renderable {
public:
    Face();
    Face(Point* p1, Point* p2, Point* p3);
    void setPoints(Point* p1, Point* p2, Point* p3);
};

class Object{
private:
    unsigned nLines;
    unsigned nPoints;
    unsigned nFaces;
    double scale[MAX_DIMENSIONS];
    double position[MAX_DIMENSIONS];
    Line lines[MAX_LINES];
    Face faces[MAX_FACES];
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
    void addToBuffer(Renderable (&renderables)[MAX_RENDERABLES], unsigned &nRenderables);
    double getScale(unsigned axis);
    double getPosition(unsigned axis);
};

#endif