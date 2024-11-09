#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "objects.h"

/*------------------------- POINT -------------------------*/
Point::Point() {}
Point::Point(double position[], unsigned dim) {
    setPosition(position, dim);
}

void Point::setPosition(double position[], unsigned dim) {
    for(int i = 0; i < dim; i++)
        setPosition(i, position[i]);
}

void Point::setPosition(unsigned index, double value) {
    if(index < MAX_DIMENSIONS)
        position[index] = value;
}

void Point::rotate(double angle, unsigned axis1, unsigned axis2) {
    double prevAxis1 = position[axis1];
    double prevAxis2 = position[axis2];

    angle = angle * (M_PI / 180);
    position[axis1] = prevAxis1 * cos(angle) - prevAxis2 * sin(angle);
    position[axis2] = prevAxis1 * sin(angle) + prevAxis2 * cos(angle);
}

double Point::getPosition(unsigned index) {
    if(index < MAX_DIMENSIONS)
        return position[index];
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

void Line::setPoint(unsigned index, Point* p) {
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

void Line::setColor(unsigned index, float value) {
    if(index > 2){
        std::cerr << "Color index in line must be only 0, 1 or 2!" << std::endl; 
        return;
    }
    color[index] = value;
}

void Line::rotate(double angle, unsigned axis1, unsigned axis2) {
    points[0]->rotate(angle, axis1, axis2);
    points[1]->rotate(angle, axis1, axis2);
}

float Line::red() { return color[0]; }
float Line::green() { return color[1]; }
float Line::blue() { return color[2]; }

Point Line::getPoint(unsigned index) {
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

void Object::setScale(double scale) {
    for(int i = 0; i < MAX_DIMENSIONS; i++) {
        for(int p = 0; p < n_points; p++)
            points[p].setPosition(i, points[p].getPosition(i) * scale / this->scale[i]);
        this->scale[i] = scale;
    }
}

void Object::rotate(double angle, unsigned axis1, unsigned axis2) {
    for(int p = 0; p < n_points; p++)
        points[p].rotate(angle, axis1, axis2);
}

double Object::getScale(unsigned index) {
    return scale[index];
}

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
                points[n].setPosition(index, std::stod(token));
                index++;
            }
            else if(state == READ_LINES_STATE) {
                if(index > 1)
                    lines[n].setColor(index - 2, std::stof(token));
                else
                {
                    int pointIndex = std::stoi(token);
                    if(pointIndex > n_points - 1)
                        std::cerr << "Invalid point index!" << std::endl;
                    else
                        lines[n].setPoint(index, &points[pointIndex]);
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

unsigned Object::linesCount() {
    return n_lines;
}

Line Object::getLine(unsigned index) {
    if(index > n_lines - 1){
        Line empty;
        std::cerr << "Invalid line index!" << std::endl; 
        return empty;
    }

    return lines[index];
}


/*------------------------- TESSERACT -------------------------*/
// void Tesseract::init(double size) {
//     for(int i = 0; i < 16; i++) {
//         double position[4];

//         position[0] = ((i % 2) / 1 * 2 - 1);
//         position[1] = ((i % 4) / 2 * 2 - 1);
//         position[2] = ((i % 8) / 4 * 2 - 1);
//         position[3] = ((i % 16)/ 8 * 2 - 1);

//         position[0] *= size/2;
//         position[1] *= size/2;
//         position[2] *= size/2;
//         position[3] *= size/2;

//         points[i].setPosition(position, 4);
//     }

//     updateLines();
// }

// void Tesseract::updateLines() {
//     // Linhas paralelas ao eixo X
//     for(int i = 0; i < 8; i++) {
//         int index = i;
//         lines[index].setPoints(
//             points[i * 2], 
//             points[i * 2 + 1]);
//         lines[index].setColor(1, 0, 0);
//     }

//     // Linhas paralelas ao eixo Y
//     for(int i = 0; i < 8; i++) {
//         int index = i + 8;
//         lines[index].setPoints(
//             points[i % 2 + (i/2) * 4], 
//             points[i % 2 + (i/2) * 4 + 2]);
//         lines[index].setColor(0, 0, 1);
//     }

//     // Linhas paralelas ao eixo Z
//     for(int i = 0; i < 8; i++) {
//         int index = i + 16;
//         lines[index].setPoints(
//             points[i % 4 + (i/4) * 8], 
//             points[i % 4 + (i/4) * 8 + 4]);
//         lines[index].setColor(0, 1, 0);
//     }

//     // Linhas paralelas ao eixo W
//     for(int i = 0; i < 8; i++) {
//         int index = i + 24;
//         lines[index].setPoints(
//             points[i % 8 + (i/8) * 16], 
//             points[i % 8 + (i/8) * 16 + 8]);
//         lines[index].setColor(1, 0, 1);
//     }
// }

// Tesseract::Tesseract() {
//     init(1);
// }
// Tesseract::Tesseract(double size) {
//     init(size);
// }

// const int Tesseract::linesCount() {
//     return 32;
// }

// Line Tesseract::getLine(unsigned index) {
//     return lines[index];
// }

// void Tesseract::rotate(double angle, unsigned axis1, unsigned axis2) {
//     for(int i = 0; i < 16; i++) {
//         points[i].rotate(angle, axis1, axis2);
//     }
//     updateLines();
// }