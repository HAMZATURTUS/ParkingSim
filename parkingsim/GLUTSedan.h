#ifndef GLUT_SEDAN_H
#define GLUT_SEDAN_H

#include <GL/glut.h>
#include <cmath>

#include "GLUTCar.h"

// toyota camry https://www.edmunds.com/toyota/camry/2025/sedan/features-specs/
class GLUTSedan : public GLUTCar {
private:
    
    // take measuremeants from a sketch of the real car and put them here relative to the length of the car (the height in the context of my program)
    float windshield_position = 0.175;
    float windshield_length = 0.26;

    float rear_window_position = 0.753;
    float rear_window_height = 0.15;

public:
    GLUTSedan(float fl[2], float angle = 0, float tyre_angle = 0, int color_choice = 0) : GLUTCar(fl, angle, tyre_angle, color_choice) {
        setHeight(4.92);
        setWidth(1.84);
        setWeight(1500);

        set_window_measuremeants(0.275, 0.167, 0.753, 0.15);
        set_tyre_measuremeants(0.149, 0.117, 0.201, 0.234);
    }


    
    
};

#endif 
