#include <GL/glut.h>

// Display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);  // Clear the screen
    glLoadIdentity();  // Reset transformations

    // Draw a red rectangle
    glColor3f(1.0, 0.0, 0.0); // Red color
    glBegin(GL_QUADS);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.5f, -0.5f);
    glVertex2f(0.5f, 0.5f);
    glVertex2f(-0.5f, 0.5f);
    glEnd();

    glFlush(); // Render the frame
}

// Initialization function
void init() {
    glClearColor(1.0, 1.0, 0.0, 1.0); // Yellow background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1); // 2D projection
    glMatrixMode(GL_MODELVIEW);
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // Single buffering with RGB mode
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(200, 100);

    glutCreateWindow("GLUT Test");

    init(); // Initialize OpenGL
    glutDisplayFunc(display); // Register display callback

    glutMainLoop(); // Start the main loop
    return 0;
}
