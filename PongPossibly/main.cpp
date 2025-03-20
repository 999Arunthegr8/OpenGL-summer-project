#include <GL/glut.h>
#include <iostream>
#include <cmath>

const float PI = 3.14159265358979323846f;

//paddle properties
float paddleX = -0.9f, paddleY = 0.0f;
const float paddleWidth = 0.04f, paddleHeight = 0.2f;
const float paddleSpeed = 0.05f;

static void display()
{
	glClearColor(0.15, 0.5, 0.9, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_QUADS);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2f(paddleX, paddleY + paddleHeight);
	glVertex2f(paddleX, paddleY);
	glColor3f(1.0, 0.0, 0.0);
	glVertex2f(paddleX + paddleWidth, paddleY);
	glVertex2f(paddleX + paddleWidth, paddleY + paddleHeight);
	glEnd();

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(0, 0);  // Center of the circle
	for (int i = 0; i <= 50; i++) {
		float angle = 2.0f * PI * i / 50;
		float x = 0 + 0.025 * cos(angle);
		float y = 0 + 0.025 * sin(angle);
		glVertex2f(x, y);
	}
	glEnd();

	glutSwapBuffers();
}

static void init()
{
	glClearColor(0.2, 0.2, 0.8, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		if (paddleY < 1)
		{
			std::cout << "move up" << std::endl;
			paddleY += paddleSpeed;
		}
		break;

	case 's':
		if (paddleY - 0.1 > -1)
		{
			std::cout << "move down" << std::endl;
			paddleY -= paddleSpeed;
		}
		break;
	}
}

void timer(int)
{
	glutPostRedisplay();
	glutTimerFunc(1000 / 60, timer, 0);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 500);
	
	glutCreateWindow("Animation");

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glutDisplayFunc(display);

	glutKeyboardFunc(keyboard);
	glutTimerFunc(0, timer, 0);

	glutMainLoop();
	return 0;
}