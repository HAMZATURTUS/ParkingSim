#ifndef GLUT_CAR_H
#define GLUT_CAR_H

#include <GL/glut.h>
#include <cmath>

/*
    https://www.asawicki.info/Mirror/Car%20Physics%20for%20Games/Car%20Physics%20for%20Games.html   engine, brake, turning physics
*/

class GLUTCar {

private:
    float color_choices[4][3] = {{0.4, 0.4, 0.4}, {0.3, 0.7, 0.3}, {0.3, 0.3, 0.7}, {0.7, 0.3, 0.3}};
    int color_choice = 0;

protected:

    float meter_px = 100.0f / 5;            // 100px = 5m

    float side_padding_wm = 0.135;
    float light_width_m = 0.585;
    float light_height_m = 0.25;

    float height_meters = 4.92;
    float width_meters = 1.84;
    float height = height_meters * meter_px;
    float width = width_meters * meter_px;
    float mass = 1500; //kg

    float tyre_height = 20;
    float tyre_width = 10;
    float tyre_offset = 0;
    float tyre_position = 15;               // how far off the front of the car is the tyre's center?
    float rear_tyre_position = 15;
    float wheelbase = height - tyre_position - rear_tyre_position;

    float windshield_position;
    float windshield_height;
    float rear_window_position;
    float rear_window_height;

    // these are for friction calculations
    const float g = 9.81;
    const float air_density = 1.29;         // kg/m^3
    float frontal_area;                     // m^2
    float cd;                               // friction coefficient;
    float crr = 0.011;                      // rolling resistance coefficient 0.011 on asphalt

    float color[3]; // r g b
    float original_color[3];

    float angle;                            // this is for the direction of the car's body
    float tyre_angle = 0;                   // i will make this value relative to the car. +angle means its pointing to the right. otherwise to the left. this value should not exceed [-45, 45]
    float max_tyre_angle = 35;
    float speed = 0;                        // ill most likely use SI units for these and convert them to px later
    float max_speed = 20;
    float acceleration = 0;
    float max_acceleration = 3.5;           // i want to turn this into a torque+gear thing but the ai will probably freak out over the automatic transmission and its too much effort for basically nothing
    float max_deceleration_force = 9000;    // the reference literally states "ill assume a constant force" and no car manufacturer wants to give me their numbers so take this
    float max_deceleration = 6.0;

    float add_speed_x = 0; // this is for the camera. it will be in px/s
    float add_speed_y = 0;

    bool brakes_on = false;
    bool lights_on = false;
    bool reverse = false;

    bool blinker_left[2] = { false };       // [is the blinker on, is the light on at this second]
    bool blinker_right[2] = { false };
    float blinker_time = 0;
    float max_blinker_time = 400;           // how many miliseconds should the light stay on

    //position
    float position_fl[2];                   // [x1, y1]
    

public:
    GLUTCar(float fl[2], float angle = 0, float tyre_angle = 0, int color_choice = 0) {

        this->color_choice = color_choice;

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
        height_meters = x;
        height = height_meters * meter_px;
    }
    void setWidth(float x){
        width_meters = x;
        width = width_meters * meter_px;
    }
    void setTyreposition(float x, float y){
        tyre_position = x;
        rear_tyre_position = y;
    }
    void setMass(float x){
        mass = x;
        max_deceleration = max_deceleration_force / mass;
    }
    float getSpeed() {
        return speed;
    }
    void getSpeeds(float& speedx, float& speedy){
        speedx = speed * std::sin(angle * M_PI / 180);
        speedy = speed * std::cos(angle * M_PI / 180);
    }
    void forceSpeeds(float speedx, float speedy) { // this is for the camera stuff
        add_speed_x = speedx;
        add_speed_y = speedy;
    }

    float getFriction() {
        return calculate_friction_force();
    }

    float getAcceleration() {
        return acceleration;
    }

    float getMeterpx() {
        return meter_px;
    }

    float getAngle() {
        return angle;
    }

    void getPosition(float& x, float& y) {
        x = position_fl[0]; y = position_fl[1];
    }

    std::string getPosition() {
        return std::to_string(position_fl[0]) + " " + std::to_string(position_fl[1]);
    }

    void set_window_measuremeants(float windshield_start, float windshield_length, float rear_window_start, float rear_window_length) {

        this->windshield_position = windshield_start * height;
        this->windshield_height = windshield_length * height;

        this-> rear_window_position = rear_window_start * height;
        this->rear_window_height = rear_window_length * height;

    }

    void set_tyre_measuremeants(float tyre_height, float tyre_width, float tyre_position, float rear_tyre_position) {

        this->tyre_height = tyre_height * height;
        this->tyre_width = tyre_width * width;
        this->tyre_position = tyre_position * height;
        this->rear_tyre_position = rear_tyre_position * height;

        wheelbase = height - this->tyre_position - this->rear_tyre_position;
    }

    void set_friction_vars(float cd, float frontal_area) {
        this->frontal_area = frontal_area;
        this->cd = cd;
    }

    void set_deceleration_force(float x) {
        max_deceleration_force = x;
    }

    void changeColor(float r, float g, float b) {
        color[0] = r; color[1] = g; color[2] = b;
    }

    void changeColor(int choice) {
        int n = sizeof(color_choices) / sizeof(color_choices[0]);
        choice %= n;
        color_choice = choice;
        changeColor(color_choices[choice][0], color_choices[choice][1], color_choices[choice][2]);
        setOriginal(color_choices[choice][0], color_choices[choice][1], color_choices[choice][2]);
    }

    void returnColor() {
        color[0] = original_color[0]; color[1] = original_color[1]; color[2] = original_color[2];
    }

    int getColorchoice() {
        return color_choice;
    }

    void setColor() {
        glColor3f(color[0], color[1], color[2]);
    }

    void setOriginal(float r, float g, float b){
        original_color[0] = r; original_color[1] = g; original_color[2] = b;
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

    void switch_gear(bool x) { // 1 for drive
        if (speed == 0){
            x = !x;
            reverse = x;
        }
    }

    void enable_blinker_left() {
        blinker_time = 0;

        if(blinker_left[0] + blinker_right[0] < 2) blinker_left[0] = !blinker_left[0];
        else blinker_left[0] = true;
        blinker_left[1] = blinker_left[0];
        blinker_right[0] = false; blinker_right[1] = false;
    }

    void enable_blinker_right() {
        blinker_time = 0;

        if(blinker_left[0] + blinker_right[0] < 2) blinker_right[0] = !blinker_right[0];
        else blinker_right[0] = true;
        blinker_right[1] = blinker_right[0];
        blinker_left[0] = false; blinker_left[1] = false;
    }

    void hazards() {
        blinker_time = 0;
        
        if(blinker_left[0] + blinker_right[0] < 2){
            blinker_right[0] = true; blinker_right[1] = true;
            blinker_left[0] = true; blinker_left[1] = true;
        }
        else{
            blinker_right[0] = false; blinker_right[1] = false;
            blinker_left[0] = false; blinker_left[1] = false;
        }
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

    void set_window_color() {
        setColor(0.1f, 0.1f, 0.3f);
    }

    void draw_windows() {

        set_window_color();
        
        float side_padding = side_padding_wm * meter_px;
        {
            float window_height = windshield_height;
            float fronty = position_fl[1] - windshield_position;
            glRectf(position_fl[0] + side_padding, fronty, position_fl[0] + width - side_padding, fronty - window_height);
        }

        {
            float window_height = rear_window_height;
            float fronty = position_fl[1] - rear_window_position;
            glRectf(position_fl[0] + side_padding, fronty, position_fl[0] + width - side_padding, fronty - rear_window_height);
        }
    }

    void draw_light_fl(float side_padding, float light_height, float light_width) {
        float fronty = position_fl[1];
        float frontx = position_fl[0] + side_padding;
        glRectf(frontx, fronty, frontx + light_width, fronty - light_height);
    }
    void draw_light_fr(float side_padding, float light_height, float light_width) {
        float fronty = position_fl[1];
        float frontx = position_fl[0] + width - side_padding - light_width;
        glRectf(frontx, fronty, frontx + light_width, fronty - light_height);
    }
    void draw_light_rl(float side_padding, float light_height, float light_width) {
        float fronty = position_fl[1] - height + light_height;
        float frontx = position_fl[0] + side_padding;
        glRectf(frontx, fronty, frontx + light_width, fronty - light_height);
    }
    void draw_light_rr(float side_padding, float light_height, float light_width) {
        float fronty = position_fl[1] - height + light_height;
        float frontx = position_fl[0] + width - side_padding - light_width;
        glRectf(frontx, fronty, frontx + light_width, fronty - light_height);
    }

    void draw_lights() {
        
        float side_padding = 0;
        float light_height = meter_px * light_height_m;
        float light_width = meter_px * light_width_m;

        // headlights
        {
            setColor(0.9f, 0.9f, 0.1f);
            {
                draw_light_fl(side_padding, light_height, light_width);
            }

            {
                draw_light_fr(side_padding, light_height, light_width);
            }
        }

        // taillights
        {
            if(brakes_on) setColor(1.0f, 0.0f, 0.0f);
            else setColor(0.7f, 0.2f, 0.2f);
            {
                draw_light_rl(side_padding, light_height, light_width);
            }

            {
                draw_light_rr(side_padding, light_height, light_width);
            }

            if(reverse) {
                setColor(1.0f, 1.0f, 1.0f);
                float light_w = light_width / 2;
                {
                    draw_light_rl(side_padding + light_width - light_w, light_height, light_w);
                }

                {
                    draw_light_rr(side_padding + light_width - light_w, light_height, light_w);
                }
            }

        }

        // blinkers
        if(blinker_left[1] + blinker_right[1]) {
            setColor(1.0f, 0.5f, 0.0f);
            if(blinker_left[1]){
                float light_w = light_width / 3;
                draw_light_fl(side_padding, light_height, light_w);
                draw_light_rl(side_padding, light_height, light_w);
            }
            if(blinker_right[1]){
                float light_w = light_width / 3;
                draw_light_fr(side_padding, light_height, light_w);
                draw_light_rr(side_padding, light_height, light_w);
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

        float ret = abs(speed / 200);
        if (speed != 0){
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
        tyre_angle += -0.1 * x;

        if(tyre_angle > 0){
            tyre_angle = std::min(max_tyre_angle, tyre_angle);
        }
        else{
            tyre_angle = std::max(-max_tyre_angle, tyre_angle);
        }

    }

    float calculate_friction_force() {

        //Fdrag =  0.5 * Cd * A * rho * v2
        float F_drag = 0.5 * cd * frontal_area * air_density * (speed * speed);
        float F_rr = crr * mass * g;

        int x = 1;
        if(speed > 0) x = -1;
        else if(speed == 0) x = 0;

        return (F_drag + F_rr) * x;

    }

    float calculate_angular_velocity() { // radians per second

        float wheelbase_m = wheelbase / meter_px;
        float r = wheelbase_m / std::sin(abs(tyre_angle) * M_PI / 180);

        int x = -1 * tyre_angle / abs(tyre_angle);
        return x * speed / r;

    }

    void accelerate() {

        release_brakes();

        float loss = calculate_friction_force() / mass;

        if (speed != loss){
            acceleration = loss;
        }
        else{
            acceleration = 0;
            speed = 0;
        }

        if(acceleration * speed < 0){
            if((abs(acceleration) >= 0.5)) press_brakes();
        }

    }

    void accelerate(float x){

        if(x < 0){
            press_brakes();
            acceleration = max_deceleration * x + (calculate_friction_force() / mass);
            
            if(speed < 0) acceleration *= -1;
            else if(speed == 0) acceleration = 0;
            return;
        }
        else release_brakes();

        acceleration = max_acceleration * x;

        if (reverse) acceleration *= -1;

        acceleration += calculate_friction_force() / mass;

    }

    void update(float time) {

        
        if(blinker_left[0] + blinker_right[0]) {
            blinker_time += time;
            if(blinker_time > max_blinker_time){
                blinker_time = 0;
                if(blinker_left[0]){
                    blinker_left[1] = !blinker_left[1];
                }
                if(blinker_right[0]){
                    blinker_right[1] = !blinker_right[1];
                }
            }
        }

        time /= 1000;
        if (time > 0.05f) time = 0.05;


        //printf("%f\n", time); 

        speed += acceleration * time;

        if (speed >= 0) {
            speed = std::min(speed, max_speed);
        }
        else if(speed < 0){
            speed = std::max(speed, -max_speed / 2);
        }

        if(abs(speed) < 0.01) speed = 0;

        float angular_v_second = calculate_angular_velocity() * 180 / M_PI; // now in degrees
        angle += angular_v_second * time;
        
        if(angle > 360) angle -= 360;
        if (angle < -360) angle += 360;

        float dist = meter_px * speed * time;
        float addx = -dist * std::sin(angle * M_PI / 180);
        float addy = dist * std::cos(angle * M_PI / 180);

        addx += add_speed_x * time;
        addy += add_speed_y * time;

        position_fl[0] += addx;
        position_fl[1] += addy;

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
