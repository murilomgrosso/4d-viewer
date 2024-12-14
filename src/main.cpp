#include <GL/glut.h>
#include <GL/gl.h>
#include <iostream>
#include <chrono>
#include "objects.h"
#include "input.h"
#include "renderer.h"

#define TARGET_FPS 60									// Frames por segundo alvo
#define TARGET_DELTATIME 1.0/TARGET_FPS					// Duracao alvo de um frame (s)
#define FPS_DISPLAY_RATE 0.5							// Tempo (s) em que o FPS eh exibido na tela
#define ROTATION_SPEED 50

int fpsCount;												// Contador de FPS				
float meanFPS;												// Armazena o valor do FPS atual medio
float fpsDisplayTimer;										// Temporizador para mostrar FPS
float deltaTime = 1.0/TARGET_FPS;							// Duracao de 1 frame (s)

std::chrono::time_point<std::chrono::system_clock> startFrameTime;	// Inicio do frame

Renderer renderer;
Object cube("solid_cube", 100);
Object tesseract("wired_tesseract", 100);

void init(int argc, char **argv);
void update(int value);
void updateView();
void draw();
double clamp(double value, double min, double max);		// Limitacao do valor

int main(int argc, char **argv) {
	startFrameTime = std::chrono::system_clock::now();
	renderer.setProjection(0, 0, 1000, 200);
	renderer.setViewport(0, 0, 500, 500);
	renderer.addObject(cube);
	renderer.addObject(tesseract);
    init(argc, argv);
}

void update(int value) 
{
	double timeLeft;	// Tempo para fim do frame (ms)
	double lastFrameTimestampCount;
	std::chrono::duration<double> timeElapsed;
	std::chrono::duration<double> lastFrameTimestamp;
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

	lastFrameTimestamp = std::chrono::duration<double>(prevStartFrame.time_since_epoch());
	lastFrameTimestampCount = lastFrameTimestamp.count();

    cube.rotate(input::getHorizontalAxis() * ROTATION_SPEED * deltaTime, 2, 0);
    cube.rotate(-input::getVerticalAxis() * ROTATION_SPEED * deltaTime, 1, 2);
    cube.rotate(-input::getMouseWheel(lastFrameTimestampCount) * ROTATION_SPEED * 4 * deltaTime, 0, 1);
    cube.rotate(input::getMouseButtonAxis() * ROTATION_SPEED * deltaTime, 3, 0);

    tesseract.rotate(input::getHorizontalAxis() * ROTATION_SPEED * deltaTime, 2, 0);
    tesseract.rotate(-input::getVerticalAxis() * ROTATION_SPEED * deltaTime, 1, 2);
    tesseract.rotate(-input::getMouseWheel(lastFrameTimestampCount) * ROTATION_SPEED * 4 * deltaTime, 0, 1);
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
	renderer.render();
}

double clamp(double value, double min, double max) 
{
	return value < min ? min : value > max ? max : value;
}