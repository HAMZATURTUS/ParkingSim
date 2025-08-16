#ifndef GLUT_SUV_H
#define GLUT_SUV_H

#include <GL/glut.h>
#include <cmath>

#include "GLUTCar.h"

// Ford Explorer 
/*
    https://www.motortrend.com/cars/ford/explorer/2025/specs?trim=XLT+2WD
    https://ecomodder.com/wiki/Vehicle_Coefficient_of_Drag_List
*/
class GLUTSuv : public GLUTCar {
private:
    

public:
    GLUTSuv(float fl[2], float angle = 0, float tyre_angle = 0, int color_choice = 0) : GLUTCar(fl, angle, tyre_angle, color_choice) {
        setHeight(5.05);
        setWidth(2);
        setMass(2000);

        float div_len = 18.9;
        set_window_measuremeants(4.6f / div_len, 2.7f / div_len, 16.5f / div_len, 1.9f / div_len);
        set_tyre_measuremeants(2.9f / div_len, 1.2f / 7.8f, 3.7f / div_len, 4.5f / div_len);
        set_friction_vars(0.43, 3.06);
    }

    
    
};

#endif 
