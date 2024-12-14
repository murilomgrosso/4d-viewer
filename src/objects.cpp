#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glut.h>
#include <GL/gl.h>
#include "objects.h"

/*------------------------- POINT -------------------------*/
Point::Point() {}
Point::Point(double position[], unsigned dim) {
    setPosition(position, dim);
}

void Point::setPosition(double position[], unsigned dim) {
    for(int i = 0; i < dim; i++)
        setPosition(position[i], i);
}

void Point::setPosition(double position, unsigned axis) {
    if(axis < MAX_DIMENSIONS)
        this->position[axis] = position;
}

double Point::getPosition(unsigned axis) {
    if(axis < MAX_DIMENSIONS)
        return position[axis];
    return 0;
}

/*------------------------- LINE -------------------------*/
Line::Line() {}
Line::Line(Point* p1, Point* p2) {
    setPoints(p1, p2);
}

void Line::setPoints(Point* p1, Point* p2) {
    points[0] = p1;
    points[1] = p2;
}

void Line::setColor(float red, float green, float blue) {
    color[0] = red;
    color[1] = green;
    color[2] = blue;
}

void Line::setColor(float value, unsigned index) {
    if(index > 2){
        std::cerr << "Color index must be only 0, 1 or 2!" << std::endl; 
        return;
    }
    color[index] = value;
}

void Line::setPoint(Point* p, unsigned index) {
    if(index > 1){
        std::cerr << "Point index in line must be 0 or 1!" << std::endl; 
        return;
    }
    points[index] = p;
}

float Line::red() { return color[0]; }
float Line::green() { return color[1]; }
float Line::blue() { return color[2]; }

Point Line::getPoint(unsigned index) {
    if(index > 1){
        Point emptyPoint;
        std::cerr << "Point index in line must be 0 or 1!" << std::endl; 
        return emptyPoint;
    }
    return *points[index];
}

/*------------------------- FACE -------------------------*/
Face::Face() {}
Face::Face(Point* p1, Point* p2, Point* p3) {
    setPoints(p1, p2, p3);
}

void Face::setPoints(Point* p1, Point* p2, Point* p3) {
    points[0] = p1;
    points[1] = p2;
    points[2] = p3;
}

void Face::setColor(float red, float green, float blue) {
    color[0] = red;
    color[1] = green;
    color[2] = blue;
}

void Face::setColor(float value, unsigned index) {
    if(index > 2){
        std::cerr << "Color index must be only 0, 1 or 2!" << std::endl; 
        return;
    }
    color[index] = value;
}

void Face::setPoint(Point* p, unsigned index) {
    if(index > 2){
        std::cerr << "Point index in face must be 0, 1 or 2!" << std::endl; 
        return;
    }
    points[index] = p;
}

float Face::red() { return color[0]; }
float Face::green() { return color[1]; }
float Face::blue() { return color[2]; }

Point Face::getPoint(unsigned index) {
    if(index > 2){
        Point emptyPoint;
        std::cerr << "Point index in face must be 0, 1 or 2!" << std::endl; 
        return emptyPoint;
    }
    return *points[index];
}

/*------------------------- OBJECTS -------------------------*/

Object::Object() {
    setScale(1.0); 
}

Object::Object(std::string object) {
    setScale(1.0);
    load(object);
}

Object::Object(std::string object, double scale) {
    setScale(1.0);
    load(object);
    setScale(scale);
}

void Object::setScale(double scale) {
    for(int i = 0; i < MAX_DIMENSIONS; i++)
        setScale(scale, i);
}

void Object::setScale(double scale, unsigned axis) {
    for(int p = 0; p < nPoints; p++)
        points[p].setPosition(points[p].getPosition(axis) * scale / this->scale[axis], axis);
    this->scale[axis] = scale;
}

void Object::setPosition(double position, unsigned axis) {
    for(int p = 0; p < nPoints; p++)
        points[p].setPosition(points[p].getPosition(axis) + position - this->position[axis], axis);
    this->position[axis] = position;
}

void Object::stretch(double factor) {
    for(int i = 0; i < MAX_DIMENSIONS; i++)
        stretch(factor, i);
}

void Object::stretch(double factor, unsigned axis) {
    setScale(scale[axis] * factor, axis);
}

void Object::translate(double amount, unsigned axis) {
    setPosition(position[axis] + amount, axis);
}

void Object::rotate(double angle, unsigned axis1, unsigned axis2) {
    double newAxis1, newAxis2, prevAxis1, prevAxis2;
    double prevPos[MAX_DIMENSIONS];
    angle = angle * (M_PI / 180);

    for(int i = 0; i < MAX_DIMENSIONS; i++) {
        prevPos[i] = position[i];
        setPosition(0, i);
    }
    
    for(int p = 0; p < nPoints; p++) {
        prevAxis1 = points[p].getPosition(axis1);
        prevAxis2 = points[p].getPosition(axis2);

        newAxis1 = prevAxis1 * cos(angle) - prevAxis2 * sin(angle);
        newAxis2 = prevAxis1 * sin(angle) + prevAxis2 * cos(angle);

        points[p].setPosition(newAxis1, axis1);
        points[p].setPosition(newAxis2, axis2);
    }

    for(int i = 0; i < MAX_DIMENSIONS; i++)
        setPosition(prevPos[i], i);
}

double Object::getScale(unsigned axis) { return scale[axis]; }
double Object::getPosition(unsigned axis) { return position[axis]; }

void Object::load(std::string object) {
    std::ifstream file(OBJECT_PATH + object + OBJECT_EXTENSION);

    if(!file.is_open()) {
        std::cerr << "Fail to open " << object << " file!" << std::endl;
        return;
    }

    int state = 0;
    unsigned n = 0;
    std::string line, token;

    while(std::getline(file, line)) {
        std::istringstream line_stream(line);
        unsigned index = 0;

        switch(state){
            case READ_POINTS_STATE:
                if(n > MAX_POINTS - 1) {
                    std::cerr << "Too many points in " << object << "!" << std::endl;
                    n++;
                    continue;
                }
                nPoints++;
                break;
            case READ_LINES_STATE:
                if(n > MAX_LINES - 1) {
                    std::cerr << "Too many lines in " << object << "!" << std::endl;
                    n++;
                    continue; 
                }
                nLines++;
                break;
            case READ_FACES_STATE:
                if(n > MAX_FACES - 1) {
                    std::cerr << "Too many faces in " << object << "!" << std::endl;
                    n++;
                    continue; 
                }
                nFaces++;
                break;
            default:
                break;
        }

        while(std::getline(line_stream, token, ' '))
        {
            if(!token.compare("#points")) {
                state = READ_POINTS_STATE;
                n = -1;
                index = 0;
            }
            else if(!token.compare("#lines")) {
                state = READ_LINES_STATE;
                n = -1;
                index = 0;
            }
            else if(!token.compare("#faces")) {
                state = READ_FACES_STATE;
                n = -1;
                index = 0;
            }
            else if(state == READ_POINTS_STATE) {
                if(index > MAX_DIMENSIONS - 1) {
                    std::cerr << "Fail to load all dimensions of " << object << "!" << std::endl;
                    break;
                }
                points[n].setPosition(std::stod(token), index);
                index++;
            }
            else if(state == READ_LINES_STATE) {
                if(index > 1)
                    lines[n].setColor(std::stof(token), index - 2);
                else
                {
                    int pointIndex = std::stoi(token);
                    if(pointIndex > nPoints - 1)
                        std::cerr << "Invalid point index!" << std::endl;
                    else
                        lines[n].setPoint(&points[pointIndex], index);
                }
                index++;
            }
            else if(state == READ_FACES_STATE) {
                if(index > 2)
                    faces[n].setColor(std::stof(token), index - 3);
                else
                {
                    int pointIndex = std::stoi(token);
                    if(pointIndex > nPoints - 1)
                        std::cerr << "Invalid point index!" << std::endl;
                    else
                        faces[n].setPoint(&points[pointIndex], index);
                }
                index++;
            }
            else {
                std::cerr << "Fail read " << object << " file!" << std::endl;
                return;
            }
        }

        n++;
    }
    file.close();
}

bool Object::getLine(Line &line, unsigned index) {
    if(index + 1 > nLines)
        return false;

    line = lines[index];

    return true;
}

bool Object::getFace(Face &face, unsigned index) {
    if(index + 1 > nFaces)
        return false;

    face = faces[index];

    return true;
}