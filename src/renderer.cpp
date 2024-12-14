#include <iostream>
#include <GL/glut.h>
#include <GL/gl.h>
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

void Renderer::insertBuffer(Object object) {
    Line line;
    Face face;

    for(int i = 0; object.getLine(line, i); i++) {
        if(nLines > MAX_LINES_RENDERS - 1) {
            std::cerr << "Line render buffer overflow!" << std::endl;
            return;
        }

        linesBuffer[nLines] = line;
        nLines++;
    }

    for(int i = 0; object.getFace(face, i); i++) {
        if(nFaces > MAX_FACE_RENDERS - 1) {
            std::cerr << "Face render buffer overflow!" << std::endl;
            return;
        }

        facesBuffer[nFaces] = face;
        nFaces++;
    }   
}

void Renderer::updateBuffer() {
    emptyBuffer();
    for(int i = 0; i < nObjects; i++)
        insertBuffer(objectsToRender[i]);
}

void Renderer::addObject(Object object) {
    if(nObjects > MAX_OBJECT_RENDERS - 1) {
        std::cerr << "Objects render buffer overflow!" << std::endl;
        return;
    }

    objectsToRender[nObjects] = object;
    nObjects++;
}