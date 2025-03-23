#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <sstream>
#include <stdbool.h>

const float PI = 3.14159265358979323846f;

//paddle properties
float paddleX = -0.9f, paddleY = 0.0f;
const float paddleWidth = 0.04f, paddleHeight = 0.2f;
const float paddleSpeed = 0.05f;

//ball properties
float ballX = 0.0f, ballY = 0.0f;
float ballRadius = 0.025f;
float ballSpeed = 0.01f;
float maxSpeed = 0.03f;
float ballDirX = -1.0f, ballDirY = 0.5f;

//game functions
int score = 0;
bool isGameOver = false;

void drawText(const char* text, float x, float y) {
    glColor3f(1.0, 1.0, 1.0);  // White text
    glRasterPos2f(x, y);        // Set position

    for (int i = 0; text[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]); // Render each character
    }
}

static void onePlayerMode()
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
	glVertex2f(ballX, ballY);  // Center of the circle
	for (int i = 0; i <= 50; i++) {
		float angle = 2.0f * PI * i / 50;
		float x = ballX + ballRadius * cos(angle);
		float y = ballY + ballRadius * sin(angle);
		glVertex2f(x, y);
	}
	glEnd();
	
	std::ostringstream scoreText;
    scoreText << "Score: " << score;
    drawText(scoreText.str().c_str(), -0.1f, 0.9f); // Position at top center
    
    if (isGameOver)
    {
    	std::ostringstream gameOverText;
    	gameOverText << "Game Over!";
    	drawText(gameOverText.str().c_str(), -0.15f, 0.7f);
	}

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
		if (paddleY + paddleHeight < 1)
		{
			std::cout << "move up" << std::endl;
			paddleY += paddleSpeed;
		}
		break;

	case 's':
		if (paddleY > -1)
		{
			std::cout << "move down" << std::endl;
			paddleY -= paddleSpeed;
		}
		break;
	}
}

void update()
{
	ballX += ballSpeed * ballDirX;
	ballY += ballSpeed * ballDirY;
	
	// top and bottom wall detection
	if(ballY + ballRadius >= 1.0f || ballY - ballRadius <=-1.0f)
	{
		ballDirY = -ballDirY;
	}
	
	
	// collision detection
	if (ballX - ballRadius <= paddleX + paddleWidth &&
		ballY >= paddleY && ballY <= paddleY + paddleHeight)
	{
		ballDirX = -ballDirX;
		if (ballSpeed < maxSpeed)
		{
			ballSpeed += 0.0025f;
		}
		score++;
	}
	
	// right wall detection
	if (ballX + ballRadius >= 1.0f)
	{
		ballDirX = -ballDirX;
	}
	
	if (ballX - ballRadius <= -1.0f)
	{
		isGameOver = true;
	}

	glutPostRedisplay();
}

void timer(int)
{
//	glutPostRedisplay();
	update();
	if (!isGameOver)
	{
		glutTimerFunc(1000 / 60, timer, 0);
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 500);
	
	glutCreateWindow("Animation");

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glutDisplayFunc(onePlayerMode);

	glutKeyboardFunc(keyboard);
	
	glutTimerFunc(0, timer, 0);

	glutMainLoop();
	return 0;
}
