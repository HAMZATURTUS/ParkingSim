#ifndef GLUT_CAR_H
#define GLUT_CAR_H

#include <GL/glut.h>
#include <cmath>

class GLUTCar {

private:
    float color_choices[3][3] = {{0.4, 0.4, 0.4}, {0.3, 0.7, 0.3}, {0.3, 0.3, 0.7}};

protected:
    float height = 100;
    float width = 50;

    int tyre_height = 20;
    int tyre_width = 10;
    int tyre_offset = 0;
    int tyre_position = 15; // how far off the front of the car is the tyre's center?
    int rear_tyre_position = 15;

    float color[3]; // r g b
    float original_color[3];

    float angle; // this is for the direction of the car
    float tyre_angle = 0; // i will make this value relative to the car. +angle means its pointing to the right. otherwise to the left. this value should not exceed [-45, 45]
    float max_tyre_angle = 45;
    float velocity = 0; // ill most likely use SI units for these and convert them to px later
    float max_velocity = 20;
    float acceleration = 0;
    float max_acceleration = 1.5;

    float meter_px = 100 / 4.5; // 100px = 4.5m

    bool brakes_on = false;
    bool lights_on = false;
    bool reverse = false;

    //position
    float position_fl[2]; // [x1, y1]
    

public:
    GLUTCar(float fl[2], float angle = 0, float tyre_angle = 0, int color_choice = 0) {

        for(int i = 0; i < 3; i++){
            color[i] = color_choices[color_choice][i];
            original_color[i] = color_choices[color_choice][i];
        }

        this->angle = angle;
        
        if (tyre_angle > max_tyre_angle) tyre_angle = max_tyre_angle;
        if (tyre_angle < -max_tyre_angle) tyre_angle = -max_tyre_angle;
        this->tyre_angle = tyre_angle;

        position_fl[0] = fl[0]; position_fl[1] = fl[1];

    }

    void setHeight(float x){
        height = x;
    }
    void setWidth(float x){
        width = x;
    }
    void setTyreposition(float x, float y){
        tyre_position = x;
        rear_tyre_position = y;
    }

    void changeColor(float r, float g, float b) {
        color[0] = r; color[1] = g; color[2] = b;
    }

    void returnColor() {
        color[0] = original_color[0]; color[1] = original_color[1]; color[2] = original_color[2];
    }

    void setColor() {
        glColor3f(color[0], color[1], color[2]);
    }

    void setColor(float r, float g, float b) {
        glColor3f(r, g, b);
    }

    void press_brakes() {
        brakes_on = true;
    }

    void release_brakes() {
        brakes_on = false;
    }

    void draw_axes() {
        setColor(0.0f, 0.0f, 0.0f);

        float multiplier = 500;

        {
            float cx = position_fl[0] + width / 2.0f;
            float cy = position_fl[1] - tyre_position;

            glPushMatrix();
                glTranslatef(cx, cy, 0.0f);             // move to center
                glRotatef(tyre_angle, 0.0f, 0.0f, 1.0f); // rotate
                glBegin(GL_LINES);
                    glVertex2f(-(width / 2.0f + multiplier), 0.0f);
                    glVertex2f( width / 2.0f + multiplier, 0.0f);
                glEnd();
            glPopMatrix();
        }
        glBegin(GL_LINES);
            glVertex2f(position_fl[0] - multiplier, position_fl[1] - (height - rear_tyre_position));
            glVertex2f(position_fl[0] + width + multiplier, position_fl[1] - (height - rear_tyre_position));
        glEnd();
    }

    void draw_tyres() {
        setColor(0.0f, 0.0f, 0.0f);

        float flx = position_fl[0] - tyre_offset;
        float fly = position_fl[1] - (tyre_position - (tyre_height / 2));

        {
            float cx = flx + tyre_width / 2.0f;  // center x
            float cy = fly - tyre_height / 2.0f; // center y

            glPushMatrix();
                glTranslatef(cx, cy, 0.0f);          // move to tire center
                glRotatef(-tyre_angle, 0.0f, 0.0f, 1.0f); // rotate about center
                glRectf(-tyre_width/2.0f,  tyre_height/2.0f, tyre_width/2.0f, -tyre_height/2.0f); // draw centered tire
            glPopMatrix();
        }

        float frx = position_fl[0] + width - tyre_width + tyre_offset;
        float fry = fly;
        
        {
            float cx = frx + tyre_width / 2.0f;  // center x
            float cy = fry - tyre_height / 2.0f; // center y

            glPushMatrix();
                glTranslatef(cx, cy, 0.0f);          // move to tire center
                glRotatef(-tyre_angle, 0.0f, 0.0f, 1.0f); // rotate about center
                glRectf(-tyre_width/2.0f,  tyre_height/2.0f, tyre_width/2.0f, -tyre_height/2.0f); // draw centered tire
            glPopMatrix();
        }

        float rlx = flx;
        float rly = position_fl[1] - height + tyre_height + rear_tyre_position - (tyre_height / 2);
        glRectf(rlx, rly, rlx + tyre_width, rly - tyre_height);

        float rrx = frx;
        float rry = rly;
        glRectf(rrx, rry, rrx + tyre_width, rry - tyre_height);

    }

    virtual void draw_windows() {
        
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

    void draw_lights() {
        
        float side_padding = 3;

        // headlights
        {
            float light_height = 5;
            float light_width = 15;
            setColor(0.9f, 0.9f, 0.1f);
            {
                float fronty = position_fl[1];
                float frontx = position_fl[0] + side_padding;
                glRectf(frontx, fronty, frontx + light_width, fronty - light_height);
            }

            {
                float fronty = position_fl[1];
                float frontx = position_fl[0] + width - side_padding - light_width;
                glRectf(frontx, fronty, frontx + light_width, fronty - light_height);
            }
        }

        // taillights
        {
            float light_height = 5;
            float light_width = 15;
            if(reverse) setColor(1.0f, 1.0f, 1.0f);
            else if(brakes_on) setColor(1.0f, 0.0f, 0.0f);
            else setColor(0.7f, 0.2f, 0.2f);
            {
                float fronty = position_fl[1] - height + light_height;
                float frontx = position_fl[0] + side_padding;
                glRectf(frontx, fronty, frontx + light_width, fronty - light_height);
            }

            {
                float fronty = position_fl[1] - height + light_height;
                float frontx = position_fl[0] + width - side_padding - light_width;
                glRectf(frontx, fronty, frontx + light_width, fronty - light_height);
            }
        }

    }

    void draw_body() {
        setColor();
        glRectf(position_fl[0], position_fl[1], position_fl[0] + width, position_fl[1] - height);

        draw_windows();
        draw_lights();

    }

    void show() {
        // Find center of the rear line (pivot for the car)
        float pivotX = position_fl[0] + width / 2.0f;
        float pivotY = position_fl[1] - (height - rear_tyre_position);

        glPushMatrix();
            // Move to pivot point
            glTranslatef(pivotX, pivotY, 0.0f);

            // Rotate the whole car
            glRotatef(angle, 0.0f, 0.0f, 1.0f);

            // Move back so that car is drawn in original coordinates
            glTranslatef(-pivotX, -pivotY, 0.0f);

            // Now draw all parts in their current local rotations
            draw_tyres();
            draw_body();
            //draw_axes();

        glPopMatrix();
    }

    

    void steer() {

        float ret = abs(velocity / 100);
        if (velocity != 0){
            if(tyre_angle > 1){
                tyre_angle -= ret;
            }
            else if(tyre_angle < -1){
                tyre_angle += ret;
            }
            else tyre_angle = 0;
        }
    }

    void steer(float x){
        tyre_angle += -0.5 * x;

        if(tyre_angle > 0){
            tyre_angle = std::min(max_tyre_angle, tyre_angle);
        }
        else{
            tyre_angle = std::max(-max_tyre_angle, tyre_angle);
        }

    }

    void accelerate() {

        release_brakes();

        float loss = 0.5;

        if (velocity > loss){
            acceleration = -loss;
        }
        else if(velocity < -loss){
            acceleration = loss;
        }
        else{
            acceleration = 0;
            velocity = 0;
        }
    }

    void accelerate(float x){

        if(x < 0){
            press_brakes();
        }
        else release_brakes();

        acceleration = max_acceleration * x;

    }

    void update(float time) {

        time /= 1000;
        if (time > 0.004f) time = 0.004;

        //printf("%f\n", time); 

        velocity += meter_px * acceleration * time;

        if (velocity >= 0) {
            velocity = std::min(velocity, max_velocity);
            reverse = false;
        }
        else if(velocity < 0){
            velocity = std::max(velocity, -max_velocity / 2);
            reverse = true;
        }

        angle += velocity / 6 * -tyre_angle * 1.1 * time;
        
        if(angle > 360) angle -= 360;
        if (angle < -360) angle += 360;

        float dist = meter_px * velocity * time;
        float addx = -dist * std::sin(angle * M_PI / 180);
        float addy = dist * std::cos(angle * M_PI / 180);

        position_fl[0] += addx;
        position_fl[1] += addy;

    }

    float getAngle() {
        return angle;
    }

    std::string getPosition() {
        return std::to_string(position_fl[0]) + " " + std::to_string(position_fl[1]);
    }

    void getCorners(float corners[4][2]) {
        float rad = angle * M_PI / 180.0f;

        // Center of rotation (rear line center)
        float pivotX = position_fl[0] + width / 2.0f;
        float pivotY = position_fl[1] - (height - rear_tyre_position);

        float local[4][2] = {
            {0, 0},            // bottom-left
            {width, 0},        // bottom-right
            {width, -height},  // top-right
            {0, -height}       // top-left
        };

        for(int i = 0; i < 4; i++) {
            float lx = local[i][0] - width/2.0f;   // translate to pivot
            float ly = local[i][1] + (height - rear_tyre_position);

            // rotate
            corners[i][0] = pivotX + lx * cos(rad) - ly * sin(rad);
            corners[i][1] = pivotY + lx * sin(rad) + ly * cos(rad);
        }
    }

    // Check collision with another car
    bool isColliding(GLUTCar* other) {
        float cornersA[4][2], cornersB[4][2];
        getCorners(cornersA);
        other->getCorners(cornersB);

        float axes[4][2];

        // 2 axes from this car (edges 0-1 and 1-2)
        for(int i=0;i<2;i++){
            float dx = cornersA[i+1][0] - cornersA[i][0];
            float dy = cornersA[i+1][1] - cornersA[i][1];
            // perpendicular
            axes[i][0] = -dy;
            axes[i][1] = dx;
        }

        // 2 axes from other car (edges 0-1 and 1-2)
        for(int i=0;i<2;i++){
            float dx = cornersB[i+1][0] - cornersB[i][0];
            float dy = cornersB[i+1][1] - cornersB[i][1];
            axes[i+2][0] = -dy;
            axes[i+2][1] = dx;
        }

        // Normalize axes
        for(int i=0;i<4;i++){
            float len = sqrt(axes[i][0]*axes[i][0] + axes[i][1]*axes[i][1]);
            axes[i][0] /= len;
            axes[i][1] /= len;
        }

        // SAT check
        for(int i=0;i<4;i++){
            float minA=1e9,maxA=-1e9;
            float minB=1e9,maxB=-1e9;

            for(int j=0;j<4;j++){
                float proj = cornersA[j][0]*axes[i][0] + cornersA[j][1]*axes[i][1];
                minA = std::min(minA, proj);
                maxA = std::max(maxA, proj);

                proj = cornersB[j][0]*axes[i][0] + cornersB[j][1]*axes[i][1];
                minB = std::min(minB, proj);
                maxB = std::max(maxB, proj);
            }

            // No overlap on this axis → no collision
            if(maxA < minB || maxB < minA)
                return false;
        }

        // All axes overlap → collision
        return true;
    }
};

#endif 
