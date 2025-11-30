#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#include <string>

#include "ParkingSim.h"

ParkingSim* sim = nullptr;
ParkingSim* ParkingSim::currentInstance = nullptr;



int main (int argc, char** argv) {
    
    glutInit(&argc, argv);
    
    
    sim = new ParkingSim("map/firstParkingLot.json");
    
    return 0;
}