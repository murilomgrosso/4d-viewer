#ifndef RENDERER_H
#define RENDERER_H

#define MAX_LINES_RENDERS 100
#define MAX_FACE_RENDERS 100
#define MAX_OBJECT_RENDERS 100

class Renderer{
private:
    unsigned nLines;
    unsigned nFaces;
    unsigned nObjects;
    Line linesBuffer[MAX_LINES_RENDERS];
    Face facesBuffer[MAX_FACE_RENDERS];
    Object objectsToRender[MAX_OBJECT_RENDERS];
    
    double xcp, ycp, zcp;
    double zvp;

    void emptyBuffer();
    void updateBuffer();
    void insertBuffer(Object object);
public:
    Renderer();
    void render();
    void addObject(Object object);
    void setViewport(int x0, int y0, int x1, int y1);
    void setProjection(double xcp, double ycp, double zcp, double zvp);
};

#endif