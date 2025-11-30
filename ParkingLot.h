#ifndef PARKING_LOT_H
#define PARKING_LOT_H

#include <GL/glut.h>
#include <cmath>

#include <json.hpp>
using json = nlohmann::json;

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <fstream>
#include <vector>
#include <string>

#include <iostream>

#include "GLUTCar.h"
#include "GLUTSedan.h"

struct tileset {

    int starting_index;
    std::string source;
    int width_px;
    int height_px;
    GLuint texture;

};

class ParkingLot {
private:
    
    float meter_px;

    float width = 100;              // meters
    float height = 100;

    float starting_position_x = 0; // where should the center of the screen be relative to the map
    float starting_position_y = 0; // also meters

    //json info
    bool infinite;

    int height_tiles; // height in number of tiles
    int width_tiles;

    int tile_height; // height of one tile in px
    int tile_width;

    int number_of_tilesets;
    std::vector<tileset> tilesets; // {starting number, name of tileset file}

    std::vector<std::vector<int>> tiles;

public:
    ParkingLot() {
        
        float *fl = new float[2];
        fl[0] = 0.0; fl[1] = 0.0;
        GLUTCar *stabilizer = new GLUTSedan(fl);
        meter_px = stabilizer->getMeterpx();
        delete stabilizer;
        delete fl;

    }

    ParkingLot(std::string file_name){

        extract_json_data(file_name);

    }

    void extract_json_data(std::string file_name){

        std::ifstream f(file_name);
        json data = json::parse(f);

        this->infinite = data["infinite"];

        this->height_tiles = data["height"];
        this->width_tiles = data["width"];
        this->tiles.resize(height_tiles);

        this->tile_height = data["tileheight"];
        this->tile_width = data["tilewidth"];

        std::vector<int> ddata = data["layers"][0]["data"];
        for(int i = 0; i < (int)ddata.size(); i++){
            this->tiles[i / width_tiles].push_back(ddata[i]);
        }

        this->number_of_tilesets = data["tilesets"].size();
        this->tilesets.resize(number_of_tilesets);

        for(int i = 0; i < this->number_of_tilesets; i++){
            tilesets[i].starting_index = data["tilesets"][i]["firstgid"];
            tilesets[i].source = data["tilesets"][i]["source"];
            tilesets[i].height_px = data["tilesets"][i]["height"];
            tilesets[i].width_px = data["tilesets"][i]["width"];
            
            int img_w, img_h, channels;
            std::string png_path = tilesets[i].source;
            unsigned char* img_data = stbi_load(png_path.c_str(), &img_w, &img_h, &channels, 0);

            GLuint tex;
            glGenTextures(1, &tex);
            glBindTexture(GL_TEXTURE_2D, tex);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
            glTexImage2D(GL_TEXTURE_2D, 0, format, img_w, img_h, 0, format, GL_UNSIGNED_BYTE, img_data);
            stbi_image_free(img_data);
            glBindTexture(GL_TEXTURE_2D, 0);

            tilesets[i].texture = tex;

        }


    }

    float getWidth() { return width * meter_px; }
    float getHeight() { return height * meter_px; }

    void getStartingPosition(float& x, float& y) {
        x = starting_position_x * meter_px;
        y = starting_position_y * meter_px;
    }
    
    
};

#endif 
