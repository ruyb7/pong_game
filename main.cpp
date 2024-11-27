#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <string>
#include <cstdlib>
#include <ctime>

const int WIDTH = 800;
const int HEIGHT = 600;

bool isPaused = false;

float paddleHeight = 100.0f;
float paddleWidth = 20.0f;
float leftPaddleY = HEIGHT / 2 - paddleHeight / 2;
float rightPaddleY = HEIGHT / 2 - paddleHeight / 2;
float paddleSpeed = 10.0f;

float ballX = WIDTH / 2;
float ballY = HEIGHT / 2;
float ballRadius = 10.0f;
float ballSpeedX = 4.0f;
float ballSpeedY = 3.0f;
float ballSpeedIncrement = 0.5f;

int leftScore = 0;
int rightScore = 0;

void drawRect(float x, float y, float width, float height) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

void drawCircle(float x, float y, float radius) {
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < 360; i++) {
        float theta = i * M_PI / 180.0f;
        glVertex2f(x + radius * cos(theta), y + radius * sin(theta));
    }
    glEnd();
}

void drawScore() {
    std::string score = std::to_string(leftScore) + " : " + std::to_string(rightScore);
    glRasterPos2f(WIDTH / 2 - 20, HEIGHT - 30);
    for (char c : score) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

void resetBall(bool scoredByLeft) {
    ballX = scoredByLeft ? paddleWidth + ballRadius : WIDTH - paddleWidth - ballRadius;
    ballY = (scoredByLeft ? leftPaddleY : rightPaddleY) + paddleHeight / 2;
    ballSpeedX = scoredByLeft ? 4.0f : -4.0f;
    ballSpeedY = 0.0f;
}

void update(int value) {
    if (!isPaused) {
        ballX += ballSpeedX;
        ballY += ballSpeedY;

        if (ballY + ballRadius > HEIGHT || ballY - ballRadius < 0) {
            ballSpeedY = -ballSpeedY;
        }

        if (ballX - ballRadius < paddleWidth &&
            ballY > leftPaddleY && ballY < leftPaddleY + paddleHeight) {
            ballSpeedX = -ballSpeedX + ballSpeedIncrement;
        }

        if (ballX + ballRadius > WIDTH - paddleWidth &&
            ballY > rightPaddleY && ballY < rightPaddleY + paddleHeight) {
            ballSpeedX = -ballSpeedX - ballSpeedIncrement;
        }

        if (ballX - ballRadius < 0) {
            rightScore++;
            resetBall(false);
        } else if (ballX + ballRadius > WIDTH) {
            leftScore++;
            resetBall(true);
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawRect(0, leftPaddleY, paddleWidth, paddleHeight);
    drawRect(WIDTH - paddleWidth, rightPaddleY, paddleWidth, paddleHeight);

    drawCircle(ballX, ballY, ballRadius);

    drawScore();

    glutSwapBuffers();
}

void handleKeys(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
            if (leftPaddleY + paddleHeight < HEIGHT) leftPaddleY += paddleSpeed;
            break;
        case 's':
            if (leftPaddleY > 0) leftPaddleY -= paddleSpeed;
            break;
        case ' ':
            isPaused = !isPaused;
            break;
    }
    glutPostRedisplay();
}

void handleSpecialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            if (rightPaddleY + paddleHeight < HEIGHT) rightPaddleY += paddleSpeed;
            break;
        case GLUT_KEY_DOWN:
            if (rightPaddleY > 0) rightPaddleY -= paddleSpeed;
            break;
    }
    glutPostRedisplay();
}

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
    glEnable(GL_DOUBLEBUFFER);

    srand(static_cast<unsigned int>(time(0)));
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Pong");

    init();

    glutDisplayFunc(render);
    glutKeyboardFunc(handleKeys);
    glutSpecialFunc(handleSpecialKeys);
    glutTimerFunc(16, update, 0);

    glutMainLoop();
    return 0;
}
