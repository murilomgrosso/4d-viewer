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

void Line::setPoint(Point* p, unsigned index) {
    if(index > 1){
        std::cerr << "Point index in line must be only 0 or 1!" << std::endl; 
        return;
    }
    points[index] = p;
}

void Line::setColor(float red, float green, float blue) {
    color[0] = red;
    color[1] = green;
    color[2] = blue;
}

void Line::setColor(float value, unsigned index) {
    if(index > 2){
        std::cerr << "Color index in line must be only 0, 1 or 2!" << std::endl; 
        return;
    }
    color[index] = value;
}

float Line::red() { return color[0]; }
float Line::green() { return color[1]; }
float Line::blue() { return color[2]; }

Point Line::getPoint(unsigned index) {
    if(index > 1){
        Point emptyPoint;
        std::cerr << "Point index in line must be only 0 or 1!" << std::endl; 
        return emptyPoint;
    }
    return *points[index];
}

/*------------------------- OBJECTS -------------------------*/

Object::Object(std::string object) {
    setScale(1.0);
    load(object);
}

Object::Object(std::string object, double scale) {
    setScale(1.0);
    load(object);
    setScale(scale);
}

void Object::draw(double xcp, double ycp, double zcp, double zvp) {
    double x, y, z, w, u;
    Point p;
    Line l;

    glBegin(GL_LINES);
        for(int i = 0; i < n_lines; i++) {
            l = lines[i];
            glColor4f(l.red(), l.green(), l.blue(), 1.0);
            for(int j = 0; j < 2; j++) {
                p = l.getPoint(j);
                x = p.getPosition(0);
                y = p.getPosition(1);
                z = p.getPosition(2);
                w = p.getPosition(3);

                u = (zcp - zvp) / (zcp - z);

                glVertex2f((1 - u) * xcp + u * x, (1 - u) * ycp + u * y); 
            }
        }
    glEnd();
}

void Object::setScale(double scale) {
    for(int i = 0; i < MAX_DIMENSIONS; i++)
        setScale(scale, i);
}

void Object::setScale(double scale, unsigned axis) {
    for(int p = 0; p < n_points; p++)
        points[p].setPosition(points[p].getPosition(axis) * scale / this->scale[axis], axis);
    this->scale[axis] = scale;
}

void Object::setPosition(double position, unsigned axis) {
    for(int p = 0; p < n_points; p++)
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
    
    for(int p = 0; p < n_points; p++) {
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
                n_points++;
                break;
            case READ_LINES_STATE:
                if(n > MAX_LINES - 1) {
                    std::cerr << "Too many lines in " << object << "!" << std::endl;
                    n++;
                    continue; 
                }
                n_lines++;
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
                    if(pointIndex > n_points - 1)
                        std::cerr << "Invalid point index!" << std::endl;
                    else
                        lines[n].setPoint(&points[pointIndex], index);
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