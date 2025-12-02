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
#include "GLUTMinivan.h"
#include "GLUTSuv.h"
#include "GLUTSedan.h"
#include "GLUTHatch.h"

#include "ParkingLot.h"

class ParkingSim {
private:

    static ParkingSim* currentInstance;
    
    float meter_px; // prone to change. it will take the value from car.h

    // window stuff
    int window_size[2] = {1600, 1000}; // [x, y]
    //int window_division[4] = {-window_size[0] / 2, window_size[0] / 2, -window_size[1] / 2, window_size[1] / 2};
    int window_division[4] = {0, window_size[0], 0, window_size[1]};
    bool text_hidden = false;


    // car stuff
    int car_choice = 0, color_choice = 0, car_choices = 5, color_choices = 4;
    float ptime = 0;
    float fl[2] = {float(window_division[0] + window_division[1]) / 2, float(window_division[2] + window_division[3]) / 2};
    GLUTCar* player = new GLUTSedan(fl);
    std::vector<GLUTCar*> others;
    int others_len;

    // text output
    GLUTOutput* outputHandler = nullptr;

    // keyboard
    bool keyStates[256] = { false };
    bool pressedLastFrame[256] = { false };

    ParkingLot* lot;

    // camera
    float cam_position_x;
    float cam_position_y;
    float cam_speed_x = 0;
    float cam_speed_y = 0;

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

        parkinglot_setup();
        
        meterpx_setup();

        std::srand(static_cast<unsigned>(std::time(nullptr))); // need to do this otherwise random generates the exact same stuff each time

        currentInstance = this;
        
        int window_size[2] = {window_size_x, window_size_y};
        for(int i = 0; i < 2; i++){
            this->window_size[i] = window_size[i];
        }

        window_division[0] = -window_size[0] / 2; window_division[1] = window_size[0] / 2; window_division[2] = -window_size[1] / 2; window_division[3] = window_size[1] / 2;
        
        glutInitWindowSize(this->window_size[0], this->window_size[1]);
        glutInitWindowPosition(0, 0);

        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);  // This stops flickering
        
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

    ParkingSim(std::string file_name, int window_size_x = 1600, int window_size_y = 1000){

        parkinglot_setup(file_name);

        meterpx_setup();

        std::srand(static_cast<unsigned>(std::time(nullptr))); // need to do this otherwise random generates the exact same stuff each time

        currentInstance = this;
        
        int window_size[2] = {window_size_x, window_size_y};
        for(int i = 0; i < 2; i++){
            this->window_size[i] = window_size[i];
        }

        window_division[0] = 0; window_division[1] = window_size[0]; window_division[2] = 0; window_division[3] = window_size[1];

        glutInitWindowSize(this->window_size[0], this->window_size[1]);
        glutInitWindowPosition(0, 0);

        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);  // This stops flickering
        
        glutCreateWindow("OpenGL testing");
        lot->initMap();

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

    void parkinglot_setup() {
        this->lot = new ParkingLot();
        lot->getStartingPosition(cam_position_x, cam_position_y);
        update_window_divisions();
    }

    void parkinglot_setup(std::string file_name){
        this->lot = new ParkingLot(file_name);
        lot->getStartingPosition(cam_position_x, cam_position_y);
        update_window_divisions();
    }

    void meterpx_setup() {
        float *fl = new float[2];
        fl[0] = 0.0; fl[1] = 0.0;
        GLUTCar *stabilizer = new GLUTSedan(fl);

        meter_px = stabilizer->getMeterpx();

        delete stabilizer;
        delete fl;
    }

    void test_field() {

        others.resize(16);
        others_len = 16;

        float fl[2] = {-70.0f * others_len / 2, -200};
        for(int i = 0; i < others_len; i++){
            others[i] = create_random(rand() % 5, fl);
            fl[0] += 70;
        }

        glutMainLoop();
    }

    void first_lot(){

        glutMainLoop();
        
    }

    void load_map();

    void change_car() {


        float angle = 0, tyre_angle = 0;
        int color_choice = player->getColorchoice();
        
        delete player;

        car_choice += 1;
        car_choice %= car_choices;
        switch (car_choice){
            case 0: player = new GLUTSedan(fl, angle, tyre_angle, color_choice); break;
            case 1: player = new GLUTHatch(fl, angle, tyre_angle, color_choice); break;
            case 2: player = new GLUTSuv(fl, angle, tyre_angle, color_choice); break;
            case 3: player = new GLUTMinivan(fl, angle, tyre_angle, color_choice); break;
            default: player = new GLUTVan(fl, angle, tyre_angle, color_choice); break;
        }
    }

    void change_color() {
        player->changeColor(player->getColorchoice() + 1);
    }

    GLUTCar* create_random(int x, float fl[2]){

        float position[2] = {fl[0], fl[1] + (rand() % 11) - 5};
        float angle = 0;
        float tyre_angle = (rand() % 91) - 45;
        int color_choice = rand() % color_choices;

        switch (x){
            case 0: return new GLUTHatch(position, angle, tyre_angle, color_choice);
            case 1: return new GLUTSedan(position, angle, tyre_angle, color_choice); 
            case 2: return new GLUTSuv(position, angle, tyre_angle, color_choice);
            case 3: return new GLUTMinivan(position, angle, tyre_angle, color_choice);
            default: return new GLUTVan(position, angle, tyre_angle, color_choice);
        }
    }

    // display functions
    void extra_info() {
        if(text_hidden == false){
            player->setColor(0.0f, 0.0f, 0.0);

            std::string messages[9] = {
                "WASD to control the car",
                "Q to reverse",
                "E to drive",
                "Z for left blinker",
                "C for right blinker",
                "X for hazards",
                "Y to change car",
                "U to change car color",
                "H to hide info"
            };
            float start = window_division[3] - 50, startx = window_division[0] + 50;
            for(std::string& message : messages){
                outputHandler->output(startx, start, message);
                start -= 30;
            }

            float x, y;
            player->getPosition(x, y);

            float s_x, s_y;
            player->getSpeeds(s_x, s_y);
            std::string divisions = "";
            for(auto division : window_division){ divisions += std::to_string(division) + " "; }
            std::string infos[11] = {
                "position " + (player->getPosition()),
                "angle " + std::to_string(player->getAngle()),
                "speed " + std::to_string(player->getSpeed()) + "m/s " + std::to_string(player->getSpeed() * 3.6) + "km/h",
                "friction " + std::to_string(player->getFriction()),
                "acceleration " + std::to_string(player->getAcceleration()),
                "length, width " + std::to_string(window_size[0]) + " " + std::to_string(window_size[1]),
                "s_x, s_y " + std::to_string(s_x) + ", " + std::to_string(s_y),
                "window divisions " + divisions,
                "position " + std::to_string(x) + " " + std::to_string(y),
                "cam position " + std::to_string(cam_position_x) + " " + std::to_string(cam_position_y),
                "lot dimensions " + std::to_string(lot->getWidth()) + " " + std::to_string(lot->getHeight()),
            };
            for(std::string& info : infos){
                outputHandler->output(startx, start, info);
                start -= 30;
            }
        }
    }

    void update_window_divisions() {
        window_division[0] = cam_position_x;
        window_division[1] = cam_position_x + window_size[0];
        window_division[2] = -cam_position_y;
        window_division[3] = -cam_position_y + window_size[1];
    }

    void update_camera_speed() {


        float x, y;
        player->getPosition(x, y);

        // s_x is negative when the car is moving to the right
        float s_x, s_y;
        player->getSpeeds(s_x, s_y);

        if(x < (window_division[0] + 300) && s_x > 0){  // moving left
            cam_speed_x = -s_x * meter_px;

            if(cam_position_x <= 0){
                cam_position_x = 0;
                cam_speed_x = 0;
            }

        }
        else if(x > (window_division[1] - 300) && s_x < 0){ // moving right
            cam_speed_x = -s_x * meter_px;

            if(cam_position_x >= (lot->getWidth() - window_size[0])){
                cam_position_x = lot->getWidth() - window_size[0];
                cam_speed_x = 0;
            }
            
        }
        else cam_speed_x = 0;

        if(y < (window_division[2] + 200) && s_y < 0) { // moving down
            cam_speed_y = -s_y * meter_px;

            if(cam_position_y >= (lot->getHeight() - window_size[1])){
                cam_position_y = lot->getHeight() - window_size[1];
                cam_speed_y = 0;
            }

        }
        else if(y > (window_division[3] - 100) && s_y > 0){ // moving up
            cam_speed_y = -s_y * meter_px;

            if(cam_position_y <= 0){
                cam_position_y = 0;
                cam_speed_y = 0;
            }
        }
        else cam_speed_y = 0;

    }

    void update_camera_position(float time) {
        time /= 1000;
        cam_position_x += cam_speed_x * time;
        cam_position_y += cam_speed_y * time;

        update_window_divisions();

        glTranslatef(-cam_position_x, cam_position_y, 0.0f);
    }

    void display() {
        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        
        // update_window_dimensions(); i forgot this doesnt even work
        update_camera_speed();
        float newtime = glutGet(GLUT_ELAPSED_TIME);
        update_camera_position(newtime - ptime);

        lot->drawMap(0, window_size[1]);

        for(GLUTCar* car : others){
            //car->forceSpeeds(-cam_speed_x, cam_speed_y);
            car->update(newtime - ptime);
            car->show();
        }
        


        //player->forceSpeeds(-cam_speed_x, cam_speed_y);
        player->update(newtime - ptime);
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
            
        ptime = newtime;

        extra_info();

        glutSwapBuffers();
    }

    void myInit() {
        
        glClearColor(1.0, 1.0, 1.0, 1.0);

        glColor3f(0.0, 1.0, 0.0);
        
        
        glPointSize(1.0);
        glMatrixMode(GL_PROJECTION); 
        glLoadIdentity();
        
        // setting window dimension in X- and Y- direction (divides the screen into these coordinates)
        //gluOrtho2D(window_division[0], window_division[1], window_division[2], window_division[3]);
        glOrtho(window_division[0], window_division[1], window_division[2], window_division[3], -1, 1);
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
        if(single_pressed('Y')){
            change_car();
        }
        if(single_pressed('U')){
            change_color();
        }

        glutPostRedisplay(); 
    }

    
    
    
};

#endif 
