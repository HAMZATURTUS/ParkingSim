/* Copyright (c) Mark J. Kilgard, 1994. */

/* This program is freely distributable without licensing fees 
   and is provided without guarantee or warrantee expressed or 
   implied. This program is -not- in the public domain. */

#ifndef GLUT_OUTPUT_H
#define GLUT_OUTPUT_H

#include <GL/glut.h>
#include <string.h>
#include <string>

class GLUTOutput {
private:
    void* font;
    void* fonts[7];
    std::string message;

public:
    GLUTOutput() {
        // Initialize fonts
        fonts[0] = GLUT_BITMAP_8_BY_13;
        fonts[1] = GLUT_BITMAP_9_BY_15;
        fonts[2] = GLUT_BITMAP_TIMES_ROMAN_10;
        fonts[3] = GLUT_BITMAP_TIMES_ROMAN_24;
        fonts[4] = GLUT_BITMAP_HELVETICA_10;
        fonts[5] = GLUT_BITMAP_HELVETICA_12;
        fonts[6] = GLUT_BITMAP_HELVETICA_18;

        
        // Default font and message
        font = fonts[3];
        message = "GLUT means OpenGL.";
    }

    void selectFont(int newfont) {
        if (newfont >= 0 && newfont < (sizeof(fonts) / sizeof(fonts[0]))) {
            font = fonts[newfont];
        }
    }

    void setMessage(const std::string& msg) {
        message = msg;
    }

    void output(int x, int y, const std::string& string) {
        glRasterPos2f(x, y);
        for (char c : string) {
            glutBitmapCharacter(font, c);
        }
    }

    // Optional: Color setting method
    void setColor(float r, float g, float b) {
        glColor3f(r, g, b);
    }
};

#endif // GLUT_OUTPUT_H
