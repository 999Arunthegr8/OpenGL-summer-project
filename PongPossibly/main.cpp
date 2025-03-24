#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <sstream>
#include <stdbool.h>

const float PI = 3.14159265358979323846f;

//paddle properties
float paddleX = -0.9f, paddleY = 0.0f;
const float paddleWidth = 0.04f, paddleHeight = 0.2f;
const float paddleSpeed = 0.027f;
//paddle movement flags
bool moveUp = false, moveDown = false;

//ball properties
float ballX = 0.0f, ballY = 0.0f;
float ballRadius = 0.025f;
float ballSpeed = 0.01f;
float maxSpeed = 0.025f;
float ballDirX = -1.0f, ballDirY = 0.5f;

//game functions
int score = 0;
bool isGameOver = false;
bool isGameRunning = false;
bool isGamePaused = false;

static void drawText(const char* text, float x, float y) {
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

	if (!isGameRunning) {
		std::ostringstream startGameText;
		startGameText << "Press 'R' to start the game.";
		drawText(startGameText.str().c_str(), -0.5f, 0.0f); // Position at center of the screen
	}

	if (isGameRunning) {
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

		if (!isGameOver) {
			//display game status
			std::ostringstream gamePausedText;
			gamePausedText << (isGamePaused ? "Paused!" : "Playing!") << " Hit 'P' to " << (isGamePaused ? "play" : "pause") << " the game.";
			drawText(gamePausedText.str().c_str(), -0.5f, -0.95f); // Position at bottom center of the screen
		}

		if (isGameOver)
		{
			std::ostringstream gameOverText;
			gameOverText << "Game Over! Hit 'R' to restart the game.";
			drawText(gameOverText.str().c_str(), -0.65f, 0.0f);
		}
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

static void update()
{
	if (!isGameRunning || isGamePaused) return;
	//smooth paddle movement
	if (moveUp && paddleY + paddleHeight < 0.95f)
	{
		paddleY += paddleSpeed;
	}

	if (moveDown && paddleY > -0.95f)
	{
		paddleY -= paddleSpeed;
	}

	ballX += ballSpeed * ballDirX;
	ballY += ballSpeed * ballDirY;

	// top and bottom wall detection
	if (ballY + ballRadius >= 1.0f || ballY - ballRadius <= -1.0f)
	{
		ballDirY = -ballDirY;
	}


	// Collision detection with paddle
	if (ballX - ballRadius <= paddleX + paddleWidth &&
		ballY >= paddleY && ballY <= paddleY + paddleHeight)
	{
		float relativeIntersectY = (ballY - paddleY) / paddleHeight - 0.5f; // Range: -0.5 to 0.5

		ballDirX = fabs(ballDirX); // Ensure it moves to the right after bounce
		ballDirY = relativeIntersectY * 2.0f; // Adjust Y direction based on hit position

		// Normalize direction to maintain speed consistency
		float length = sqrt(ballDirX * ballDirX + ballDirY * ballDirY);
		ballDirX /= length;
		ballDirY /= length;

		if (ballSpeed < maxSpeed && score % 3 == 0)
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

static void timer(int)
{
	//	glutPostRedisplay();
	if (isGameRunning && !isGameOver)
	{
		update();
		glutTimerFunc(1000 / 60, timer, 0);
	}
}


void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		moveUp = true;
		break;

	case 's':
		moveDown = true;
		break;

	case 'r':
		std::cout << "Game is up" << std::endl;
		if (!isGameRunning || isGameOver) {
			isGameRunning = true;
			isGameOver = false;
			score = 0;
			ballX = 0.0f;
			ballY = 0.0f;
			ballSpeed = 0.01f;
			ballDirX = -1.0f;
			ballDirY = 0.5f;
			glutTimerFunc(0, timer, 0); // Restart the game loop
		}
		break;

	case 'p':
		isGamePaused = !isGamePaused;
		std::cout << "Game is " << (isGamePaused ? "paused" : "being played") << std::endl;
		glutPostRedisplay(); // force a redraw to display updated status text
		break;
	}
}

// function to handle key release
static void keyboardUp(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		moveUp = false;
		break;

	case 's':
		moveDown = false;
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 500);

	glutCreateWindow("Possibly Pong");

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glutDisplayFunc(onePlayerMode);

	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);

	glutTimerFunc(0, timer, 0);

	glutMainLoop();
	return 0;
}
