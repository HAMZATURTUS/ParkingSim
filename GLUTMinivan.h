#ifndef GLUT_MINIVAN_H
#define GLUT_MINIVAN_H

#include <GL/glut.h>
#include <cmath>

#include "GLUTCar.h"

// Dodge Caravan
/*
	https://www.caranddriver.com/dodge/grand-caravan/specs		dimensions

*/
class GLUTMinivan : public GLUTCar {
private:
    

public:
    GLUTMinivan(float fl[2], float angle = 0, float tyre_angle = 0, int color_choice = 0) : GLUTCar(fl, angle, tyre_angle, color_choice) {
        setHeight(5.15);
        setWidth(2);
        setMass(2000);

        set_window_measuremeants(118.0f / 581.0f, (209.0f - 118.0f) / 581.0f, 503.0f / 581.0f, (554.0f - 503.0f) / 581.0f);
        set_tyre_measuremeants(80.0f / 581.0f, 0.9f / 6.5f, 109.0f / 581.0f, (581.0f - 458.0f) / 581.0f);
        set_friction_vars(0.35, 2.85); 
    }

    
    
};

#endif 
