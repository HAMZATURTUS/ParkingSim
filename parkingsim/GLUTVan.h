#ifndef GLUT_VAN_H
#define GLUT_VAN_H

#include <GL/glut.h>
#include <cmath>

#include "GLUTCar.h"

class GLUTVan : public GLUTCar {
private:
    

public:
    GLUTVan(float fl[2], float angle = 0, float tyre_angle = 0, float r = 1, float g = 0, float b = 0) : GLUTCar(fl, angle, tyre_angle, r, g, b) {
        setHeight(110);
        setTyreposition(15, 20);
    }


    

    void draw_body() {
        setColor();
        glRectf(position_fl[0], position_fl[1], position_fl[0] + width, position_fl[1] - height);

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
