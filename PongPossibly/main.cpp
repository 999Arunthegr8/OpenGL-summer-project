//two players 2d ping pong
#include <GL/glut.h>
#include <stdio.h>

float ballX = 0.0f, ballY = 0.0f, ballDirX = 0.02f, ballDirY = 0.01f;
float paddle1Y = 0.0f, paddle2Y = 0.0f;
int score1 = 0, score2 = 0;

void drawText(float x, float y, char* string) {
    glRasterPos2f(x, y);
    while (*string)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *string++);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Set background color (blue)
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Draw border (white)
    glColor3f(0.0f, 0.0f, 0.0f);  // White color for border
    glBegin(GL_LINES);
    glVertex2f(-1.0f, 1.0f);
    glVertex2f(1.0f, 1.0f);

    glVertex2f(1.0f, 1.0f);
    glVertex2f(1.0f, -1.0f);

    glVertex2f(1.0f, -1.0f);
    glVertex2f(-1.0f, -1.0f);

    glVertex2f(-1.0f, -1.0f);
    glVertex2f(-1.0f, 1.0f);
    glEnd();

    // Draw ball
    glColor3f(0.921f, 0.203f, 0.835f);  //
    glBegin(GL_QUADS);
    glVertex2f(ballX - 0.02f, ballY - 0.02f);
    glVertex2f(ballX + 0.02f, ballY - 0.02f);
    glVertex2f(ballX + 0.02f, ballY + 0.02f);
    glVertex2f(ballX - 0.02f, ballY + 0.02f);
    glEnd();

    // Draw paddles (left: red, right: red)
    glColor3f(1.0f, 0.0f, 0.0f);  // Red color for paddles
    glBegin(GL_QUADS);
    glVertex2f(-0.9f, paddle1Y - 0.1f);
    glVertex2f(-0.88f, paddle1Y - 0.1f);
    glVertex2f(-0.88f, paddle1Y + 0.1f);
    glVertex2f(-0.9f, paddle1Y + 0.1f);

    glVertex2f(0.88f, paddle2Y - 0.1f);
    glVertex2f(0.9f, paddle2Y - 0.1f);
    glVertex2f(0.9f, paddle2Y + 0.1f);
    glVertex2f(0.88f, paddle2Y + 0.1f);
    glEnd();

    // Display score
    char scoreText[20];
    sprintf_s(scoreText, "%d - %d", score1, score2);
    drawText(-0.05f, 0.9f, scoreText);

    glutSwapBuffers();
}

void update(int value) {
    ballX += ballDirX;
    ballY += ballDirY;

    // Collision with top and bottom walls
    if (ballY > 1.0f || ballY < -1.0f)
        ballDirY = -ballDirY;

    // Collision with paddles
    if ((ballX < -0.88f && ballY < paddle1Y + 0.1f && ballY > paddle1Y - 0.1f) ||
        (ballX > 0.88f && ballY < paddle2Y + 0.1f && ballY > paddle2Y - 0.1f)) {
        ballDirX = -ballDirX;
    }

    // Scoring
    if (ballX < -1.0f) {
        score2++;
        ballX = 0.0f;
        ballY = 0.0f;
        ballDirX = 0.02f;
        if (score2 >= 10) {
            printf("Player 2 wins! Game Over!\n");
            exit(0);  // Ends the game
        }
    }
    if (ballX > 1.0f) {
        score1++;
        ballX = 0.0f;
        ballY = 0.0f;
        ballDirX = -0.02f;
        if (score1 >= 10) {
            printf("Player 1 wins! Game Over!\n");
            exit(0);  // Ends the game
        }
    }


    glutPostRedisplay();
    glutTimerFunc(30, update, 0);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'w' && paddle1Y < 0.9f) paddle1Y += 0.1f;
    if (key == 's' && paddle1Y > -0.9f) paddle1Y -= 0.1f;
    if (key == 'o' && paddle2Y < 0.9f) paddle2Y += 0.1f;
    if (key == 'l' && paddle2Y > -0.9f) paddle2Y -= 0.1f;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Ping Pong Game");

    glOrtho(-1, 1, -1, 1, -1, 1);

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, update, 0);

    glutMainLoop();
    return 0;
}