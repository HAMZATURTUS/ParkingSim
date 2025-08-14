#ifndef GLUT_HATCH_H
#define GLUT_HATCH_H

#include <GL/glut.h>
#include <cmath>

#include "GLUTCar.h"

class GLUTHatch : public GLUTCar {
private:
    

public:
    GLUTHatch(float fl[2], float angle = 0, float tyre_angle = 0, int color_choice = 0) : GLUTCar(fl, angle, tyre_angle, color_choice) {
        setHeight(90);
        setWeight(1100);
    }
    

    void draw_windows() {

        {
            float side_padding = 5;
            float window_height = 15;
            float fronty = position_fl[1] - (5 + window_height);
            set_window_color();
            glRectf(position_fl[0] + side_padding, fronty, position_fl[0] + width - side_padding, fronty - window_height);
        }

        {
            float side_padding = 5;
            float window_height = 10;
            float fronty = position_fl[1] - (height - 15);
            set_window_color();
            glRectf(position_fl[0] + side_padding, fronty, position_fl[0] + width - side_padding, fronty - window_height);
        }
    }

    
    
};

#endif 
