// CoreGameEngine.cpp : Defines the entry point for the console application.
//

#if __APPLE__
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <stdio.h>
#include <string.h>

#else
#include "stdafx.h"
#include "../Dependencies/glew/glew.h"
#include "../Dependencies/freeglut/freeglut.h"
#endif

#include <iostream>
#include "math_3d.h"

GLuint VBO;

void renderScene(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(0);

	glutSwapBuffers();
}

static void InitializeGlutCallbacks()
{
	glutDisplayFunc(renderScene);
}

static void CreateVertexBuffer()
{
	Vector3f Vertices[3];
	Vertices[0] = Vector3f(-1.0f, -1.0f, 0.0f);
	Vertices[1] = Vector3f(1.0f, -1.0f, 0.0f);
	Vertices[2] = Vector3f(0.0f, 1.0f, 0.0f);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
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
	InitializeGlutCallbacks();

	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
        
        std::cout << "Error: %s\n" << glewGetErrorString(res) << std::endl;
		return 1;
	}


	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	CreateVertexBuffer();



	glutMainLoop();

	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
	return 0;
}
