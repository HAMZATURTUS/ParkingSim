#ifndef GLUT_SEDAN_H
#define GLUT_SEDAN_H

#include <GL/glut.h>
#include <cmath>

#include "GLUTCar.h"

class GLUTSedan : public GLUTCar {
private:
    

public:
    GLUTSedan(float fl[2], float angle = 0, float tyre_angle = 0, int color_choice = 0) : GLUTCar(fl, angle, tyre_angle, color_choice) {
        setHeight(100);
    }
    

    void draw_windows() {

        
        {
            float side_padding = 5;
            float window_height = 20;
            float fronty = position_fl[1] - 25;
            setColor(0.1f, 0.1f, 0.3f);
            glRectf(position_fl[0] + side_padding, fronty, position_fl[0] + width - side_padding, fronty - window_height);
        }

        {
            float side_padding = 5;
            float window_height = 15;
            float fronty = position_fl[1] - 70;
            setColor(0.1f, 0.1f, 0.3f);
            glRectf(position_fl[0] + side_padding, fronty, position_fl[0] + width - side_padding, fronty - window_height);
        }
    }

    
    
};

#endif 
