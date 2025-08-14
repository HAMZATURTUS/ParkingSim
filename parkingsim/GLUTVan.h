#ifndef GLUT_VAN_H
#define GLUT_VAN_H

#include <GL/glut.h>
#include <cmath>

#include "GLUTCar.h"

class GLUTVan : public GLUTCar {
private:
    

public:
    GLUTVan(float fl[2], float angle = 0, float tyre_angle = 0, int color_choice = 0) : GLUTCar(fl, angle, tyre_angle, color_choice) {
        setHeight(110);
        setTyreposition(15, 20);
    }
    

    void draw_windows() {

        {
            float side_padding = 5;
            float window_height = 20;
            float fronty = position_fl[1] - 20;
            setColor(0.1f, 0.1f, 0.3f);
            glRectf(position_fl[0] + side_padding, fronty, position_fl[0] + width - side_padding, fronty - window_height);
        }
    }

    
    
};

#endif 
