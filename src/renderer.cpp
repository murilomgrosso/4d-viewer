/*
TODO:
    descartar faces fora da view
    descartar faces que nao estao viradas para camera
    ordenar buffer de renderizacao
    verificar se está na frente ou atras da camera
    distancia n dimensional?
*/

#include <iostream>
#include <GL/glut.h>
#include <GL/gl.h>
#include <cmath>
#include "objects.h"
#include "renderer.h"

Renderer::Renderer() {}

void Renderer::render() {
	Face face;
	Line line;
    Point p;
	double x, y, z, w, u;

    updateBuffer();

    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.192, 0.133, 0.173, 1.0);

	glBegin(GL_LINES);
	for(int i = 0; i < nLines; i++) {
		line = linesBuffer[i];
		glColor4f(line.red(), line.green(), line.blue(), 1.0);
		for(int j = 0; j < 2; j++) {
			p = line.getPoint(j);
			x = p.getPosition(0);
			y = p.getPosition(1);
			z = p.getPosition(2);
			w = p.getPosition(3);

			u = (zcp - zvp) / (zcp - z);

			glVertex2f((1 - u) * xcp + u * x, (1 - u) * ycp + u * y); 
		}
	}
	glEnd();

	glBegin(GL_TRIANGLES);
	for(int i = 0; i < nFaces; i++) {
		face = facesBuffer[i];
		glColor4f(face.red(), face.green(), face.blue(), 1.0);
		for(int j = 0; j < 3; j++) {
			p = face.getPoint(j);
			x = p.getPosition(0);
			y = p.getPosition(1);
			z = p.getPosition(2);
			w = p.getPosition(3);

			u = (zcp - zvp) / (zcp - z);

			glVertex2f((1 - u) * xcp + u * x, (1 - u) * ycp + u * y); 
		}
	}
	glEnd();

    glFlush();
}

void Renderer::setProjection(double xcp, double ycp, double zcp, double zvp) {
    this->xcp = xcp;
    this->ycp = ycp;
    this->zcp = zcp;
    this->zvp = zvp;
}

void Renderer::setViewport(int x0, int y0, int x1, int y1) {
    glViewport(x0, y0, x1, y1);
}

void Renderer::emptyBuffer() {
    nLines = 0;
    nFaces = 0;
}

void Renderer::insertLineBuffer(Line line) {
    if(nLines > MAX_LINES_RENDERS - 1) {
        std::cerr << "Line render buffer overflow!" << std::endl;
        return;
    }

    int l = 0;
    int r = nLines;
    int m = (l + r) / 2;

    double mDist;
    double dist = distToCamera(line.getCentralPoint());

    while(l < r) {
        mDist = distToCamera(linesBuffer[m].getCentralPoint());

        if(dist < mDist)
            l = m + 1;
        else if(dist > mDist)
            r = m;
        else
            break;

        m = (l + r) / 2;
    }

    Line temp;

    nLines++;
    for(int i = m; i < nLines; i++) {
        temp = linesBuffer[i];
        linesBuffer[i] = line;
        line = temp;
    }
}

void Renderer::insertFaceBuffer(Face face) {
    if(nFaces > MAX_FACE_RENDERS - 1) {
        std::cerr << "Face render buffer overflow!" << std::endl;
        return;
    }

    int l = 0;
    int r = nFaces;
    int m = (l + r) / 2;

    double mDist;
    double dist = distToCamera(face.getCentralPoint());

    while(l < r) {
        mDist = distToCamera(facesBuffer[m].getCentralPoint());

        if(dist < mDist)
            l = m + 1;
        else if(dist > mDist)
            r = m;
        else
            break;

        m = (l + r) / 2;
    }

    Face temp;

    nFaces++;
    for(int i = m; i < nFaces; i++) {
        temp = facesBuffer[i];
        facesBuffer[i] = face;
        face = temp;
    }
}

void Renderer::insertObjectBuffer(Object object) {
    Line line;
    Face face;

    for(int i = 0; object.getLine(line, i); i++)
        insertLineBuffer(line);

    for(int i = 0; object.getFace(face, i); i++)
        insertFaceBuffer(face);   
}

void Renderer::updateBuffer() {
    emptyBuffer();
    for(int i = 0; i < nObjects; i++)
        insertObjectBuffer(objectsToRender[i]);
}

void Renderer::addObject(Object object) {
    if(nObjects > MAX_OBJECT_RENDERS - 1) {
        std::cerr << "Objects render buffer overflow!" << std::endl;
        return;
    }

    objectsToRender[nObjects] = object;
    nObjects++;
}

double Renderer::distToCamera(Point p) {
    // POR ENQUANTO APENAS DISTANCIA 3D
    double dist = 0;

    dist += std::pow(xcp - p.getPosition(0), 2);
    dist += std::pow(ycp - p.getPosition(1), 2);
    dist += std::pow(zcp - p.getPosition(2), 2);
    dist = std::sqrt(dist);

    return dist;
}