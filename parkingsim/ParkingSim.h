#ifndef PARKING_SIM_H
#define PARKING_SIM_H


#include <cmath>
#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#include <string>
#include <random>
#include <ctime>

#include "GLUTOutput.h"
#include "GLUTCar.h"
#include "GLUTVan.h"
#include "GLUTSuv.h"
#include "GLUTSedan.h"
#include "GLUTHatch.h"

class ParkingSim {
private:

    static ParkingSim* currentInstance;
    
    // window stuff
    int window_size[2] = {1600, 1000}; // [x, y]
    int window_division[4] = {-window_size[0] / 2, window_size[0] / 2, -window_size[1] / 2, window_size[1] / 2};
    bool text_hidden = false;


    // car stuff
    float time = 0;
    float fl[2] = {0.0, 0.0};
    GLUTCar* player = new GLUTSedan(fl);
    GLUTCar* others[16];
    int others_len = sizeof(others) / sizeof(others[0]);

    // text output
    GLUTOutput* outputHandler = nullptr;

    // keyboard
    bool keyStates[256] = { false };
    bool pressedLastFrame[256] = { false };


    // these make the constructor work
    static void displayCallback() {
        if (currentInstance) 
            currentInstance->display();
    }

    static void keyDownCallback(unsigned char key, int x, int y) {
        if (currentInstance) 
            currentInstance->keyDown(key, x, y);
    }

    static void keyUpCallback(unsigned char key, int x, int y) {
        if (currentInstance) 
            currentInstance->keyUp(key, x, y);
    }

    static void keyboardUpdateCallback() {
        if (currentInstance) 
            currentInstance->keyboard_update();
    }

public:
    ParkingSim(int window_size_x = 1600, int window_size_y = 1000) {
        

        std::srand(static_cast<unsigned>(std::time(nullptr))); // needs <cstdlib> + <ctime>

        currentInstance = this;
        
        int window_size[2] = {window_size_x, window_size_y};
        for(int i = 0; i < 2; i++){
            this->window_size[i] = window_size[i];
        }

        window_division[0] = -window_size[0] / 2; window_division[1] = window_size[0] / 2; window_division[2] = -window_size[1] / 2; window_division[3] = window_size[1] / 2;

        glutInitWindowSize(this->window_size[0], this->window_size[1]);
        glutInitWindowPosition(0, 0);
        
        glutCreateWindow("OpenGL testing");
        myInit();

        outputHandler = new GLUTOutput(6);

        glutDisplayFunc(displayCallback);
        glutIdleFunc(keyboardUpdateCallback); 
        //glutKeyboardFunc(handleKeyboard);

        glutKeyboardFunc(keyDownCallback);
        glutKeyboardUpFunc(keyUpCallback);


        //glutMainLoop();

    }

    ~ParkingSim() {
        for(GLUTCar* car : others) delete car;
        delete player;
        delete outputHandler;
    }

    void test_field() {

        float fl[2] = {-70.0f * others_len / 2, -200};
        for(int i = 0; i < others_len; i++){
            others[i] = create_random(rand() % 4, fl);
            fl[0] += 70;
        }

        glutMainLoop();
    }

    GLUTCar* create_random(int x, float fl[2]){

        float position[2] = {fl[0], fl[1] + (rand() % 11) - 5};
        float angle = 0;
        float tyre_angle = (rand() % 91) - 45;
        int color_choice = rand() % 4;

        switch (x){
            case 0: return new GLUTHatch(position, angle, tyre_angle, color_choice);
            case 1: return new GLUTSedan(position, angle, tyre_angle, color_choice);
            case 2: return new GLUTSuv(position, angle, tyre_angle, color_choice);
            default: return new GLUTVan(position, angle, tyre_angle, color_choice);
        }
    }

    // display functions
    void extra_info() {
        if(text_hidden == false){
            player->setColor(0.0f, 0.0f, 0.0);

            std::string messages[7] = {
                "WASD to control the car",
                "Q to reverse",
                "E to drive",
                "Z for left blinker",
                "C for right blinker",
                "X for hazards",
                "H to hide"
            };
            float start = 450, startx = window_division[0] + 50;
            for(std::string& message : messages){
                outputHandler->output(startx, start, message);
                start -= 30;
            }

            std::string infos[6] = {
                "position " + (player->getPosition()),
                "angle " + std::to_string(player->getAngle()),
                "speed " + std::to_string(player->getSpeed()) + "m/s " + std::to_string(player->getSpeed() * 3.6) + "km/h",
                "friction " + std::to_string(player->getFriction()),
                "acceleration " + std::to_string(player->getAcceleration()),
                "length, width " + std::to_string(window_size[0]) + " " + std::to_string(window_size[1])
            };
            for(std::string& info : infos){
                outputHandler->output(startx, start, info);
                start -= 30;
            }
        }
    }

    void update_window_dimensions() {
        this->window_size[0] = glutGet(GLUT_WINDOW_WIDTH);
        this->window_size[1] = glutGet(GLUT_SCREEN_HEIGHT);

        window_division[0] = -window_size[0] / 2; window_division[1] = window_size[0] / 2; window_division[2] = -window_size[1] / 2; window_division[3] = window_size[1] / 2;
        //gluOrtho2D(window_division[0], window_division[1], window_division[2], window_division[3]);

    }

    void display() {
        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        update_window_dimensions();
        
        for(GLUTCar* car : others){
            car->show();
        }

        float newtime = glutGet(GLUT_ELAPSED_TIME);
        player->update(newtime - time);
        player->show();

        {
            bool problem = false;
            for(GLUTCar* car : others){
                if (player->isColliding(car)) {
                    problem = true;
                }
            }
            if(problem){
                player->changeColor(0.0f, 1.0f, 0.0);
            }
            else player->returnColor();
        }
            
        time = newtime;

        extra_info();

        glFlush();
    }

    void myInit() {
        
        glClearColor(1.0, 1.0, 1.0, 1.0);

        glColor3f(0.0, 1.0, 0.0);
        
        
        glPointSize(1.0);
        glMatrixMode(GL_PROJECTION); 
        glLoadIdentity();
        
        // setting window dimension in X- and Y- direction (divides the screen into these coordinates)
        gluOrtho2D(window_division[0], window_division[1], window_division[2], window_division[3]);
    }


    // keyboard functions
    void keyDown(unsigned char key, int x, int y) {
        keyStates[key] = true;
    }

    void keyUp(unsigned char key, int x, int y) {
        keyStates[key] = false;
    }

    bool single_pressed(char x) {
        bool ret = false;
        bool CurrentlyPressed = keyStates[x + 32] || keyStates[x];
        if (CurrentlyPressed && !pressedLastFrame[x]) { // this extra stuff should prevent the weird behaviour when q is held down for more than 0.1 seconds
            ret = true;
        }
        pressedLastFrame[x] = CurrentlyPressed;
        return ret;
    }

    bool continuous_pressed(char x) {
        return (keyStates[x] || keyStates[x + 32]);
    }

    void keyboard_update() {

        if (continuous_pressed('A')) {
            player->steer(1.0f);
        }
        else if (continuous_pressed('D')) {
            player->steer(-1.0f);
        }
        else player->steer();

        if (continuous_pressed('W')) {
            player->accelerate(1.0f);
        }
        else if (continuous_pressed('S')) {
            player->accelerate(-1.0f);
        }
        else player->accelerate();

        
        if(single_pressed('Q')){
            player->switch_gear(0);
        }
        if(single_pressed('E')){
            player->switch_gear(1);
        }
        if(single_pressed('H')){
            text_hidden = !text_hidden;
        }
        if(single_pressed('Z')){
            player->enable_blinker_left();
        }
        if(single_pressed('C')){
            player->enable_blinker_right();
        }
        if(single_pressed('X')){
            player->hazards();
        }

        glutPostRedisplay(); 
    }

    
    
    
};

#endif 
