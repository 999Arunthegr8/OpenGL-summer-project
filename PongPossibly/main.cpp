#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <sstream>
#include <stdbool.h>
// Window dimensions (for two player)
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

const float PI = 3.14159265358979323846f;

// ONE PLAYER MODE PROPERTIES
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

static void onePlayerDisplay()
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

static void onePlayerInit()
{
	glClearColor(0.2, 0.2, 0.8, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
}

static void onePlayerUpdate()
{
	if (!isGameRunning || isGamePaused) return;
	//smooth paddle movement
	if (moveUp && paddleY + paddleHeight < 0.98f)
	{
		paddleY += paddleSpeed;
	}

	if (moveDown && paddleY > -0.975f)
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

static void onePlayerTimer(int)
{
	//	glutPostRedisplay();
	if (isGameRunning && !isGameOver)
	{
		onePlayerUpdate();
		glutTimerFunc(1000 / 60, onePlayerTimer, 0);
	}
}


void onePlayerKeyboard(unsigned char key, int x, int y)
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
		if (!isGameRunning || isGameOver) {
			isGameRunning = true;
			isGameOver = false;
			score = 0;
			ballX = 0.0f;
			ballY = 0.0f;
			ballSpeed = 0.01f;
			ballDirX = -1.0f;
			ballDirY = 0.5f;
			glutTimerFunc(0, onePlayerTimer, 0); // Restart the game loop
		}
		break;

	case 'p':
		isGamePaused = !isGamePaused;
		glutPostRedisplay(); // force a redraw to display onePlayerUpdated status text
		break;
	}
}

// function to handle key release
static void onePlayerKeyboardUp(unsigned char key, int x, int y)
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

// TWO PLAYER MODE
typedef struct {
    float x, y;       // Position
    float width, height;  // Dimensions
    float dx, dy;     // Direction
    float speed;      // Speed
    int score;        // Score
} Paddle;

typedef struct {
    float x, y;       // Position
    float radius;     // Ball radius
    float dx, dy;     // Direction
    float speed;      // Speed
    float baseSpeed;  // Base speed to reset to when a point is scored
    float maxSpeed;   // Maximum speed the ball can reach
    float speedIncrement; // How much to increase the speed after each hit
    int hitCounter;   // Counter for consecutive hits without scoring
} Ball;

// Game state
typedef enum {
    GAME_START,
    GAME_PLAYING,
    GAME_PAUSED,
    GAME_OVER
} GameState;

// Global variables
Paddle paddleLeft, paddleRight;
Ball ball;
GameState gameState = GAME_START;
int winningScore = 10;
bool keys[256];

// Function prototypes
void init(void);
void display(void);
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);
void specialKeys(int key, int x, int y);
void specialKeysUp(int key, int x, int y);
void timer(int value);
void resetGame(void);
void resetBall(bool goingRight);
void drawText(float x, float y, const char* string);
void drawPaddle(Paddle paddle, float r, float g, float b);
void drawBall(Ball ball, float r, float g, float b);
void updateGame(void);
void checkCollision(void);

// Initialize the game
void init(void) {
    // Set background color to white
    glClearColor(1.0, 1.0, 1.0, 1.0);

    // Initialize paddles
    paddleLeft.width = 10;
    paddleLeft.height = 80;
    paddleLeft.x = 20;
    paddleLeft.y = WINDOW_HEIGHT / 2 - paddleLeft.height / 2;
    paddleLeft.speed = 5.0;
    paddleLeft.score = 0;

    paddleRight.width = 10;
    paddleRight.height = 80;
    paddleRight.x = WINDOW_WIDTH - 20 - paddleRight.width;
    paddleRight.y = WINDOW_HEIGHT / 2 - paddleRight.height / 2;
    paddleRight.speed = 5.0;
    paddleRight.score = 0;

    // Initialize ball
    ball.radius = 10.0;
    ball.x = WINDOW_WIDTH / 2;
    ball.y = WINDOW_HEIGHT / 2;
    ball.dx = -1.0;
    ball.dy = 0.0;
    ball.baseSpeed = 3.0;  // Starting speed
    ball.speed = ball.baseSpeed;
    ball.maxSpeed = 10.0;  // Maximum speed the ball can reach
    ball.speedIncrement = 0.2;  // Increase speed by this much after each hit
    ball.hitCounter = 0;   // Initialize hit counter

    // Initialize game state
    gameState = GAME_START;

    // Initialize key states
    memset(keys, 0, sizeof(keys));
}

// Reset the ball after a point is scored
void resetBall(bool goingRight) {
    ball.x = WINDOW_WIDTH / 2;
    ball.y = WINDOW_HEIGHT / 2;
    ball.dx = goingRight ? 1.0 : -1.0;
    ball.dy = 0.0;
    ball.speed = ball.baseSpeed;  // Reset to base speed when a point is scored
    ball.hitCounter = 0;          // Reset hit counter
}

// Draw text on the screen
void drawText(float x, float y, const char* string) {
    glColor3f(0.0, 0.0, 0.0); // Black text
    glRasterPos2f(x, y);

    for (int i = 0; i < strlen(string); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
    }
}

// Draw a paddle
void drawPaddle(Paddle paddle, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(paddle.x, paddle.y);
    glVertex2f(paddle.x + paddle.width, paddle.y);
    glVertex2f(paddle.x + paddle.width, paddle.y + paddle.height);
    glVertex2f(paddle.x, paddle.y + paddle.height);
    glEnd();
}

// Draw the ball
void drawBall(Ball ball, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(ball.x, ball.y); // Center
    for (int i = 0; i <= 360; i += 10) {
        float angle = i * 3.14159f / 180.0f;
        glVertex2f(ball.x + ball.radius * cosf(angle), ball.y + ball.radius * sinf(angle));
    }
    glEnd();
}

// Display function
void display(void) {
    char scoreText[100];
    char speedText[100];

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw paddles (red color)
    drawPaddle(paddleLeft, 1.0, 0.0, 0.0);  // Red paddle
    drawPaddle(paddleRight, 1.0, 0.0, 0.0); // Red paddle

    // Draw ball (orange color)
    drawBall(ball, 1.0, 0.5, 0.0);  // Orange ball

    // Draw score
    sprintf_s(scoreText, "Score: %d - %d", paddleLeft.score, paddleRight.score);
    drawText(WINDOW_WIDTH / 2 - 70, 30, scoreText);

    // Draw ball speed (new)
    sprintf_s(speedText, "Ball Speed: %.1f", ball.speed);
    drawText(WINDOW_WIDTH / 2 - 70, 60, speedText);

    // Draw game state messages
    if (gameState == GAME_START) {
        drawText(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2, "Press SPACE to start the game");
    }
    else if (gameState == GAME_PAUSED) {
        drawText(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2, "GAME PAUSED - Press P to resume");
    }
    else if (gameState == GAME_OVER) {
        char winnerText[100];
        if (paddleLeft.score >= winningScore) {
            sprintf_s(winnerText, "Player 1 Wins! Score: %d - %d", paddleLeft.score, paddleRight.score);
        }
        else {
            sprintf_s(winnerText, "Player 2 Wins! Score: %d - %d", paddleLeft.score, paddleRight.score);
        }

        drawText(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 30, winnerText);
        drawText(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2, "Press R to restart or Q to quit");
    }

    // Display controls info at the bottom of the screen
    drawText(10, WINDOW_HEIGHT - 20, "Controls: W/S - Left Paddle, Arrow Up/Down - Right Paddle, P - Pause/Resume");

    glutSwapBuffers();
}

// Reshape function
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    glMatrixMode(GL_MODELVIEW);
}

// Keyboard function for key press
void keyboard(unsigned char key, int x, int y) {
    keys[key] = true;

    if (key == 27) { // ESC key
        exit(0);
    }

    if (key == ' ' && gameState == GAME_START) {
        gameState = GAME_PLAYING;
    }

    if (key == 'p' || key == 'P') {
        if (gameState == GAME_PLAYING) {
            gameState = GAME_PAUSED;
        }
        else if (gameState == GAME_PAUSED) {
            gameState = GAME_PLAYING;
        }
    }

    if (key == 'r' && gameState == GAME_OVER) {
        resetGame();
    }

    if (key == 'q' && gameState == GAME_OVER) {
        exit(0);
    }
}

// Keyboard function for key release
void keyboardUp(unsigned char key, int x, int y) {
    keys[key] = false;
}

// Special keys function for key press
void specialKeys(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        keys[GLUT_KEY_UP] = true;
        break;
    case GLUT_KEY_DOWN:
        keys[GLUT_KEY_DOWN] = true;
        break;
    }
}

// Special keys function for key release
void specialKeysUp(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        keys[GLUT_KEY_UP] = false;
        break;
    case GLUT_KEY_DOWN:
        keys[GLUT_KEY_DOWN] = false;
        break;
    }
}

// Check for collisions
void checkCollision(void) {
    // Ball collision with top and bottom
    if (ball.y - ball.radius <= 0 || ball.y + ball.radius >= WINDOW_HEIGHT) {
        ball.dy = -ball.dy;
    }

    // Ball collision with left paddle
    if (ball.x - ball.radius <= paddleLeft.x + paddleLeft.width &&
        ball.x - ball.radius >= paddleLeft.x &&
        ball.y >= paddleLeft.y &&
        ball.y <= paddleLeft.y + paddleLeft.height) {

        // Simple predictable reflection - just reverse x direction and apply small angle based on position
        ball.dx = fabs(ball.dx);  // Make sure it's moving right

        // Calculate a more controlled angle based on where ball hits paddle
        // Dividing paddle into 5 sections for predictable angles
        float paddleSection = paddleLeft.height / 5;
        float hitPos = ball.y - paddleLeft.y;

        if (hitPos < paddleSection) {
            // Top section - go up at fixed angle
            ball.dy = -0.7;
        }
        else if (hitPos < 2 * paddleSection) {
            // Upper middle - go slightly up
            ball.dy = -0.3;
        }
        else if (hitPos < 3 * paddleSection) {
            // Middle - go straight
            ball.dy = 0;
        }
        else if (hitPos < 4 * paddleSection) {
            // Lower middle - go slightly down
            ball.dy = 0.3;
        }
        else {
            // Bottom section - go down at fixed angle
            ball.dy = 0.7;
        }

        // Increase ball speed and hitCounter after a successful hit
        ball.hitCounter++;
        if (ball.speed < ball.maxSpeed) {
            ball.speed += ball.speedIncrement;
            if (ball.speed > ball.maxSpeed) ball.speed = ball.maxSpeed;
        }
    }

    // Ball collision with right paddle
    if (ball.x + ball.radius >= paddleRight.x &&
        ball.x + ball.radius <= paddleRight.x + paddleRight.width &&
        ball.y >= paddleRight.y &&
        ball.y <= paddleRight.y + paddleRight.height) {

        // Simple predictable reflection - just reverse x direction and apply small angle based on position
        ball.dx = -fabs(ball.dx);  // Make sure it's moving left

        // Calculate a more controlled angle based on where ball hits paddle
        // Dividing paddle into 5 sections for predictable angles
        float paddleSection = paddleRight.height / 5;
        float hitPos = ball.y - paddleRight.y;

        if (hitPos < paddleSection) {
            // Top section - go up at fixed angle
            ball.dy = -0.7;
        }
        else if (hitPos < 2 * paddleSection) {
            // Upper middle - go slightly up
            ball.dy = -0.3;
        }
        else if (hitPos < 3 * paddleSection) {
            // Middle - go straight
            ball.dy = 0;
        }
        else if (hitPos < 4 * paddleSection) {
            // Lower middle - go slightly down
            ball.dy = 0.3;
        }
        else {
            // Bottom section - go down at fixed angle
            ball.dy = 0.7;
        }

        // Increase ball speed and hitCounter after a successful hit
        ball.hitCounter++;
        if (ball.speed < ball.maxSpeed) {
            ball.speed += ball.speedIncrement;
            if (ball.speed > ball.maxSpeed) ball.speed = ball.maxSpeed;
        }
    }

    // Ball out of bounds (left)
    if (ball.x - ball.radius <= 0) {
        paddleRight.score++;
        if (paddleRight.score >= winningScore) {
            gameState = GAME_OVER;
        }
        else {
            resetBall(true);  // Reset ball going right
        }
    }

    // Ball out of bounds (right)
    if (ball.x + ball.radius >= WINDOW_WIDTH) {
        paddleLeft.score++;
        if (paddleLeft.score >= winningScore) {
            gameState = GAME_OVER;
        }
        else {
            resetBall(false);  // Reset ball going left
        }
    }
}

// Update game state
void updateGame(void) {
    // Only update game if it's in PLAYING state
    if (gameState == GAME_PLAYING) {
        // Left paddle movement (W and S keys)
        if (keys['w'] || keys['W']) {
            paddleLeft.y -= paddleLeft.speed;
        }
        if (keys['s'] || keys['S']) {
            paddleLeft.y += paddleLeft.speed;
        }

        // Right paddle movement (Up and Down arrow keys)
        if (keys[GLUT_KEY_UP]) {
            paddleRight.y -= paddleRight.speed;
        }
        if (keys[GLUT_KEY_DOWN]) {
            paddleRight.y += paddleRight.speed;
        }

        // Keep paddles within bounds
        if (paddleLeft.y < 0) paddleLeft.y = 0;
        if (paddleLeft.y + paddleLeft.height > WINDOW_HEIGHT) paddleLeft.y = WINDOW_HEIGHT - paddleLeft.height;

        if (paddleRight.y < 0) paddleRight.y = 0;
        if (paddleRight.y + paddleRight.height > WINDOW_HEIGHT) paddleRight.y = WINDOW_HEIGHT - paddleRight.height;

        // Update ball position
        ball.x += ball.dx * ball.speed;
        ball.y += ball.dy * ball.speed;

        // Check for collisions
        checkCollision();
    }
}

// Timer function for animation
void timer(int value) {
    updateGame();
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // ~60 FPS
}

// Reset the game
void resetGame(void) {
    paddleLeft.score = 0;
    paddleRight.score = 0;

    ball.x = WINDOW_WIDTH / 2;
    ball.y = WINDOW_HEIGHT / 2;
    ball.dx = -1.0;
    ball.dy = 0.0;
    ball.speed = ball.baseSpeed;  // Reset to base speed
    ball.hitCounter = 0;          // Reset hit counter

    gameState = GAME_START;
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);


	int mode = 0, isRunning = true;
	std::cout<<"Choose player mode: "<<std::endl
			<<"1. One-Player"<<std::endl
			<<"2. Two-Player"<<std::endl
			<<"3. Quit Game"<<std::endl;
	std::cin>>mode;

	switch (mode)
	{
	case 1:
		glutInitWindowPosition(100, 100);
		glutInitWindowSize(500, 500);

		glutCreateWindow("Possibly Pong One Player");
		glClearColor(0.0, 0.0, 0.0, 1.0);
	
		glutDisplayFunc(onePlayerDisplay);
		glutKeyboardFunc(onePlayerKeyboard);
		glutKeyboardUpFunc(onePlayerKeyboardUp);
		glutTimerFunc(0, onePlayerTimer, 0);
		break;
			
	case 2:
		std::cout<<"Two-player"<<std::endl;
		glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
   		glutInitWindowPosition(100, 100);
   		glutCreateWindow("Possibly Pong Two Player");

   		init();

   		glutDisplayFunc(display);
   		glutReshapeFunc(reshape);
   		glutKeyboardFunc(keyboard);
   		glutKeyboardUpFunc(keyboardUp);
   		glutSpecialFunc(specialKeys);
   		glutSpecialUpFunc(specialKeysUp);
   		glutTimerFunc(0, timer, 0);
		break;
			
	case 3:
		std::cout<<"Leaving game...Thank you for playing!"<<std::endl;
		isRunning = false;
	
	default:
		std::cout<<"invalid choice"<<std::endl;
		break;
	}
	
	

	glutMainLoop();
	return 0;
}
