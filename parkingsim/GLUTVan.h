#ifndef GLUT_VAN_H
#define GLUT_VAN_H

#include <GL/glut.h>
#include <cmath>

#include "GLUTCar.h"

// GMC Savana https://www.caranddriver.com/gmc/savana/specs
class GLUTVan : public GLUTCar {
private:
    

public:
    GLUTVan(float fl[2], float angle = 0, float tyre_angle = 0, int color_choice = 0) : GLUTCar(fl, angle, tyre_angle, color_choice) {
        setHeight(5.7);
        setWidth(2);
        setMass(2750);

        set_window_measuremeants(2.3f / 17.0f, 2.0f / 17.0f, 16.0f / 17.0f, 0.6f / 17.0f);
        set_tyre_measuremeants(2.4f / 17.0f, 0.9f / 6.5f, 2.7f / 17.0f, 3.5f / 17.0f);
        set_friction_vars(0.43, 4.33); // estimated
    }

    
    
};

#endif 
