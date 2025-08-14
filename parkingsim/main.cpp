#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#include <string>
#define pi 3.142857

#include "GLUTOutput.h"
#include "GLUTCar.h"
#include "GLUTVan.h"
#include "GLUTSuv.h"
#include "GLUTSedan.h"
GLUTOutput* outputHandler = nullptr;
GLUTCar* anycar = nullptr;
GLUTCar* othercars[2];

float time = 0;
bool hidden = false;

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

    if(hidden == false){
        anycar->setColor(0.0f, 0.0f, 0.0);

        outputHandler->output(-750, 450, "WASD to control the car");
        outputHandler->output(-750, 420, "Q to reverse");
        outputHandler->output(-750, 390, "E to drive");
        outputHandler->output(-750, 360, "H to hide");

        int start = 330;
        outputHandler->output(-750, start, "position " + (anycar->getPosition()));
        outputHandler->output(-750, start - 30, "angle " + std::to_string(anycar->getAngle()));
        outputHandler->output(-750, start - 30 * 2, "velocity " + std::to_string(anycar->getVelocity()));
    }

    for(GLUTCar* car : othercars){
        car->show();
    }
    float newtime = glutGet(GLUT_ELAPSED_TIME);
    anycar->update(newtime - time);
    anycar->show();

    {
        bool problem = false;
        for(GLUTCar* car : othercars){
            if (anycar->isColliding(car)) {
                problem = true;
            }
        }
        if(problem){
            anycar->changeColor(0.0f, 1.0f, 0.0);
        }
        else anycar->returnColor();
    }
        
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

bool pressedLastFrame[26];

bool single_pressed(char x) {
    bool ret = false;
    bool CurrentlyPressed = keyStates[x + 32] || keyStates[x];
    if (CurrentlyPressed && !pressedLastFrame[x - 'A']) { // this extra stuff should prevent the weird behaviour when q is held down for more than 0.1 seconds
        ret = true;
    }
    pressedLastFrame[x - 'A'] = CurrentlyPressed;
    return ret;
}

void update(void) {

    if (keyStates['a'] || keyStates['A']) {
        anycar->steer(1.0f);
    }
    else if (keyStates['d'] || keyStates['D']) {
        anycar->steer(-1.0f);
    }
    else anycar->steer();

    if (keyStates['w'] || keyStates['W']) {
        anycar->accelerate(1.0f);
    }
    else if (keyStates['s'] || keyStates['S']) {
        anycar->accelerate(-1.0f);
    }
    else anycar->accelerate();

    
    if(single_pressed('Q')){
        anycar->switch_gear(0);
    }

    if(single_pressed('E')){
        anycar->switch_gear(1);
    }

    if(single_pressed('H')){
        hidden = !hidden;
    }

    glutPostRedisplay(); 
}


int main (int argc, char** argv) {
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    
    // giving window size in X- and Y- direction
    glutInitWindowSize(1400, 800);
    glutInitWindowPosition(0, 0);

    for(int i = 0; i < 26; i++){
        pressedLastFrame[i] = false;
    }

    outputHandler = new GLUTOutput();
    outputHandler->selectFont(6);

    float fl[2] = {0.0, 0.0};
    anycar = new GLUTSedan(fl);
    fl[0] = 0.0; fl[1] = -200.0;

    othercars[0] = new GLUTVan(fl, 0, 40, 1);

    fl[0] = 70.0; fl[1] = -200.0;
    othercars[1] = new GLUTSuv(fl, 0, -20, 2);
    
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