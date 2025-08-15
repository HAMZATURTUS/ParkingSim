#ifndef GLUT_HATCH_H
#define GLUT_HATCH_H

#include <GL/glut.h>
#include <cmath>

#include "GLUTCar.h"

// toyota yaris https://www.carsguide.com.au/toyota/yaris/car-dimensions
class GLUTHatch : public GLUTCar {
private:
    

public:
    GLUTHatch(float fl[2], float angle = 0, float tyre_angle = 0, int color_choice = 0) : GLUTCar(fl, angle, tyre_angle, color_choice) {
        setHeight(3.95);
        setWidth(1.7);
        setWeight(1100);

        set_window_measuremeants(0.267, 0.188, 0.83, 0.121);
        set_tyre_measuremeants(0.147, 0.149, 0.215, 0.175);
    }
    
    
};

#endif 
