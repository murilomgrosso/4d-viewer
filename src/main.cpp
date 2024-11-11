#include <GL/glut.h>
#include <GL/gl.h>
#include <iostream>
#include <chrono>
#include "objects.h"
#include "input.h"

#define TARGET_FPS 60									// Frames por segundo alvo
#define TARGET_DELTATIME 1.0/TARGET_FPS					// Duracao alvo de um frame (s)
#define FPS_DISPLAY_RATE 0.5							// Tempo (s) em que o FPS eh exibido na tela
#define ROTATION_SPEED 50
#define MAX_LINES_RENDERS 100
#define MAX_FACE_RENDERS 100

int fpsCount;												// Contador de FPS				
float meanFPS;												// Armazena o valor do FPS atual medio
float fpsDisplayTimer;										// Temporizador para mostrar FPS
float deltaTime = 1.0/TARGET_FPS;							// Duracao de 1 frame (s)

std::chrono::time_point<std::chrono::system_clock> startFrameTime;	// Inicio do frame

Object cube("solid_cube", 100);
Object tesseract("wired_tesseract", 100);

void init(int argc, char **argv);
void update(int value);
void updateView();
void draw();
void renderObject(Object object, Line (&lines)[MAX_LINES_RENDERS], Face (&faces)[MAX_FACE_RENDERS], unsigned &nLines, unsigned &nFaces);
double clamp(double value, double min, double max);		// Limitacao do valor

int main(int argc, char **argv) {
	startFrameTime = std::chrono::system_clock::now();
    init(argc, argv);
}

void update(int value) 
{
	double timeLeft;	// Tempo para fim do frame (ms)
	std::chrono::duration<double> timeElapsed;
	std::chrono::time_point<std::chrono::system_clock> prevStartFrame;

	/* Calcula o valor do FPS atual */
	prevStartFrame = startFrameTime;
	startFrameTime = std::chrono::system_clock::now();
	timeElapsed = (startFrameTime - prevStartFrame);
	deltaTime = timeElapsed.count();
	meanFPS += 1.0/deltaTime;
	fpsCount++;

	if(fpsDisplayTimer < 0) 
	{
		//std::cout << "FPS: " << (int)(meanFPS/fpsCount) << std::endl;
		fpsDisplayTimer = FPS_DISPLAY_RATE;
		meanFPS = 0;
		fpsCount = 0;
	}
	else
		fpsDisplayTimer -= deltaTime;

    cube.rotate(input::getHorizontalAxis() * ROTATION_SPEED * deltaTime, 2, 0);
    cube.rotate(-input::getVerticalAxis() * ROTATION_SPEED * deltaTime, 1, 2);
    //cube.rotate(-input::getMouseWheel() * ROTATION_SPEED * deltaTime, 0, 1);
    cube.rotate(input::getMouseButtonAxis() * ROTATION_SPEED * deltaTime, 3, 0);

    tesseract.rotate(input::getHorizontalAxis() * ROTATION_SPEED * deltaTime, 2, 0);
    tesseract.rotate(-input::getVerticalAxis() * ROTATION_SPEED * deltaTime, 1, 2);
    //tesseract.rotate(-input::getMouseWheel() * ROTATION_SPEED * deltaTime, 0, 1);
    tesseract.rotate(input::getMouseButtonAxis() * ROTATION_SPEED * deltaTime, 3, 0);

	/* Atualiza a view */
	updateView();

	/* Chama o proximo frame */
	timeElapsed = std::chrono::system_clock::now() - startFrameTime;
	timeLeft = TARGET_DELTATIME * 1000 - timeElapsed.count();
	timeLeft = clamp(timeLeft, 0, timeLeft);

	glutTimerFunc(timeLeft, update, value + 1);
}

void updateView() {
	glutPostRedisplay();
}

void init(int argc, char **argv) {
    glutInit(&argc, argv);    
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowPosition(800,200);
    glutInitWindowSize(500,500);
    glutCreateWindow("");

    glutDisplayFunc(draw);

	glutMouseFunc(input::mouseManager);
	glutKeyboardFunc(input::keyboardDown);
	glutKeyboardUpFunc(input::keyboardUp);
	glutSpecialFunc(input::specialKeysDown);
	glutSpecialUpFunc(input::specialKeysUp);

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glLineWidth(5.0);
    gluOrtho2D(-250.0, 250.0, -250.0, 250.0);

    update(0);
    glutMainLoop();
}

void draw()
{
	Face face;
	Line line;
    Point p;

    double xcp = 0;
    double ycp = 0;
    double zcp = 1000;
    double zvp = 200;
    double x, y, z, w, u;

	unsigned nLines = 0;
	unsigned nFaces = 0;
	Line lines[MAX_LINES_RENDERS];
	Face faces[MAX_FACE_RENDERS];

    glViewport(0, 0, 500, 500);

    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.192, 0.133, 0.173, 1.0);

	renderObject(cube, lines, faces, nLines, nFaces);

	glBegin(GL_LINES);
	for(int i = 0; i < nLines; i++) {
		line = lines[i];
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
		face = faces[i];
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

void renderObject(Object object, Line (&lines)[MAX_LINES_RENDERS], Face (&faces)[MAX_FACE_RENDERS], unsigned &nLines, unsigned &nFaces) {
	Face face;
	Line line;

	glBegin(GL_LINES);
	for(int i = 0; object.getLine(line, i); i++) {
		if(nLines > MAX_LINES_RENDERS - 1) {
			std::cerr << "Line render buffer overflow!" << std::endl;
			return;
		}

		lines[nLines] = line;
		nLines++;
	}
	glEnd();

	glBegin(GL_TRIANGLES);
	for(int i = 0; object.getFace(face, i); i++) {
		if(nFaces > MAX_FACE_RENDERS - 1) {
			std::cerr << "Face render buffer overflow!" << std::endl;
			return;
		}

		faces[nFaces] = face;
		nFaces++;
	}
	glEnd();
}

double clamp(double value, double min, double max) 
{
	return value < min ? min : value > max ? max : value;
}