#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#include <string>
#define pi 3.142857

#include "GLUTOutput.h"
#include "GLUTCar.h"
GLUTOutput* outputHandler = nullptr;
GLUTCar* anycar = nullptr;

float time = 0;

// function to initialize
void myInit (void)
{
    
    glClearColor(1.0, 1.0, 1.0, 1.0);

    glColor3f(0.0, 1.0, 0.0);
    
    
    glPointSize(1.0);
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();
    
    // setting window dimension in X- and Y- direction (divides the screen into these coordinates)
    gluOrtho2D(-800, 800, -500, 500);
}

void display (void) 
{
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    anycar->setColor(0.0f, 1.0f, 0.0);
    std::string x = "angle " + std::to_string(anycar->getAngle());
    outputHandler->output(-750, 450, x);

    float newtime = glutGet(GLUT_ELAPSED_TIME);
    anycar->update(newtime - time);
    anycar->show();
    
    time = newtime;

    glFlush();
}

bool keyStates[256] = { false };

void keyDown(unsigned char key, int x, int y) {
    keyStates[key] = true;
}

void keyUp(unsigned char key, int x, int y) {
    keyStates[key] = false;
}

void update(void) {

    if (keyStates['a'] || keyStates['A']) {
        anycar->steer(-1.0f);
    }
    else if (keyStates['d'] || keyStates['D']) {
        anycar->steer(1.0f);
    }
    else anycar->steer();

    if (keyStates['w'] || keyStates['W']) {
        anycar->accelerate(1.0f);
    }
    else if (keyStates['s'] || keyStates['S']) {
        anycar->accelerate(-1.0f);
    }
    else anycar->accelerate();

    glutPostRedisplay(); 
}


int main (int argc, char** argv) {
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    
    // giving window size in X- and Y- direction
    glutInitWindowSize(1400, 800);
    glutInitWindowPosition(0, 0);

    outputHandler = new GLUTOutput();

    float fl[2] = {0.0, 0.0};
    anycar = new GLUTCar(fl);
    
    // Giving name to window
    glutCreateWindow("OpenGL testing");
    myInit();
    
    glutDisplayFunc(display);
    glutIdleFunc(update); 
    //glutKeyboardFunc(handleKeyboard);

    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);


    glutMainLoop();
}