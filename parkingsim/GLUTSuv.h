#ifndef GLUT_SUV_H
#define GLUT_SUV_H

#include <GL/glut.h>
#include <cmath>

#include "GLUTCar.h"

class GLUTSuv : public GLUTCar {
private:
    

public:
    GLUTSuv(float fl[2], float angle = 0, float tyre_angle = 0, int color_choice = 0) : GLUTCar(fl, angle, tyre_angle, color_choice) {
        setHeight(105);
    }


    

    void draw_windows() {

        {
            float side_padding = 5;
            float window_height = 15;
            float fronty = position_fl[1] - (5 + window_height);
            setColor(0.1f, 0.1f, 0.3f);
            glRectf(position_fl[0] + side_padding, fronty, position_fl[0] + width - side_padding, fronty - window_height);
        }

        {
            float side_padding = 5;
            float window_height = 10;
            float fronty = position_fl[1] - 90;
            setColor(0.1f, 0.1f, 0.3f);
            glRectf(position_fl[0] + side_padding, fronty, position_fl[0] + width - side_padding, fronty - window_height);
        }
    }

    
    
};

#endif 
