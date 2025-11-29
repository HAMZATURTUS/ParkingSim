#ifndef PARKING_LOT_H
#define PARKING_LOT_H

#include <GL/glut.h>
#include <cmath>

#include <json.hpp>

#include "GLUTCar.h"
#include "GLUTSedan.h"

class ParkingLot {
private:
    
    float meter_px;

    float width = 100;              // meters
    float height = 100;

    float starting_position_x = 50; // where should the center of the screen be relative to the map
    float starting_position_y = 50; // also meters

public:
    ParkingLot() {
        
        float *fl = new float[2];
        fl[0] = 0.0; fl[1] = 0.0;
        GLUTCar *stabilizer = new GLUTSedan(fl);
        meter_px = stabilizer->getMeterpx();
        delete stabilizer;
        delete fl;

    }

    float getWidth() { return width * meter_px; }
    float getHeight() { return height * meter_px; }

    void getStartingPosition(float& x, float& y) {
        x = starting_position_x * meter_px;
        y = starting_position_y * meter_px;
    }
    
    
};

#endif 
