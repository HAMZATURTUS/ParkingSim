#ifndef GLUT_SUV_H
#define GLUT_SUV_H

#include <GL/glut.h>
#include <cmath>

#include "GLUTCar.h"

class GLUTSuv : public GLUTCar {
private:
    

public:
    GLUTSuv(float fl[2], float angle = 0, float tyre_angle = 0, float r = 1, float g = 0, float b = 0) : GLUTCar(fl, angle, tyre_angle, r, g, b) {
        
    }


    

    void draw_body() {
        setColor();
        glRectf(position_fl[0], position_fl[1], position_fl[0] + width, position_fl[1] - height);

        {
            float side_padding = 5;
            float window_height = 20;
            float fronty = position_fl[1] - 25;
            setColor(0.1f, 0.1f, 0.3f);
            glRectf(position_fl[0] + side_padding, fronty, position_fl[0] + width - side_padding, fronty - window_height);
        }

        {
            float side_padding = 5;
            float window_height = 10;
            float fronty = position_fl[1] - 85;
            setColor(0.1f, 0.1f, 0.3f);
            glRectf(position_fl[0] + side_padding, fronty, position_fl[0] + width - side_padding, fronty - window_height);
        }
    }

    
    
};

#endif 
