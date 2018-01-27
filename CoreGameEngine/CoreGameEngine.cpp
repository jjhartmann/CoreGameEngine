// CoreGameEngine.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "../Dependencies/glew/glew.h"
#include "../Dependencies/freeglut/freeglut.h"

#include "math_3d.h"

void renderScene(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0, 0.0, 0.0, 1.0);//clear red

	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	// Init glut
	glutInit(&argc, argv);

	// Configure Display
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("GameEngine");

	// register callbacks
	glutDisplayFunc(renderScene);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glutMainLoop();

	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
	return 0;
}
