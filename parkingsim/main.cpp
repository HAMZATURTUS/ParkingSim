#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#include <string>

#include "ParkingSim.h"

ParkingSim* sim = nullptr;
ParkingSim* ParkingSim::currentInstance = nullptr;



int main (int argc, char** argv) {
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    
    sim = new ParkingSim;
    sim->test_field();
    
    return 0;
}