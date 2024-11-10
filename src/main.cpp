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

int fpsCount;												// Contador de FPS				
float meanFPS;												// Armazena o valor do FPS atual medio
float fpsDisplayTimer;										// Temporizador para mostrar FPS
float deltaTime = 1.0/TARGET_FPS;							// Duracao de 1 frame (s)

std::chrono::time_point<std::chrono::system_clock> startFrameTime;	// Inicio do frame

Object cube("cube", 100);

void init(int argc, char **argv);
void update(int value);
void updateView();
void draw();
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
    cube.rotate(-input::getMouseWheel() * ROTATION_SPEED * deltaTime, 0, 1);
    cube.rotate(input::getMouseButtonAxis() * ROTATION_SPEED * deltaTime, 3, 0);

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
    double xcp = 0;
    double ycp = 0;
    double zcp = 1000;
    double zvp = 200;

    glViewport(0, 0, 500, 500);

    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.192, 0.133, 0.173, 1.0);

    cube.draw(xcp, ycp, zcp, zvp);

    glFlush();
}

double clamp(double value, double min, double max) 
{
	return value < min ? min : value > max ? max : value;
}