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

};

struct colorcode {
    int r;
    int g;
    int b;
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

    std::vector<std::vector<int>> tile_ids;

    std::vector<std::vector<std::vector<std::vector<int>>>> tiles;

    GLuint full_map_texture = 0;   // one big texture for the whole map
    int map_pixel_w = 0;
    int map_pixel_h = 0;

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

    bool load_image(std::vector<unsigned char>& image, const std::string& filename, int& x, int&y){
        int n;
        unsigned char* data = stbi_load(filename.c_str(), &x, &y, &n, 4);
        if (data != nullptr)
        {
            image = std::vector<unsigned char>(data, data + x * y * 4);
        }
        stbi_image_free(data);
        return (data != nullptr);
    }

    void getColorAtPixel(std::vector<unsigned char> image, int x, int y, int img_w, int& r, int& g, int& b){
        colorcode ret;
        const size_t RGBA = 4;

        size_t index = RGBA * (y * img_w + x);
        
        r = image[index];
        g = image[index + 1];
        b = image[index + 2];

    }
    
    void extract_json_data(std::string file_name){

        std::ifstream f(file_name);
        json data = json::parse(f);

        this->infinite = data["infinite"];

        this->height_tiles = data["height"];
        this->width_tiles = data["width"];
        this->tile_ids.resize(height_tiles);

        this->tile_height = data["tileheight"];
        this->tile_width = data["tilewidth"];

        std::vector<int> ddata = data["layers"][0]["data"];
        for(int i = 0; i < (int)ddata.size(); i++){
            this->tile_ids[i / width_tiles].push_back(ddata[i]);
        }

        this->number_of_tilesets = data["tilesets"].size();
        this->tilesets.resize(number_of_tilesets);

        tiles.resize(500);
        for(int i = 0; i < this->number_of_tilesets; i++){
            tilesets[i].starting_index = data["tilesets"][i]["firstgid"];
            tilesets[i].source = data["tilesets"][i]["source"];
            tilesets[i].height_px = data["tilesets"][i]["height"];
            tilesets[i].width_px = data["tilesets"][i]["width"];
            
            int img_w, img_h;
            std::string png_path = tilesets[i].source;
            std::vector<unsigned char> image;
            load_image(image, png_path, img_w, img_h);


            int idx = tilesets[i].starting_index;
            for(int ii = 0; ii < tilesets[i].height_px; ii += this->tile_height){
                for(int j = 0; j < tilesets[i].width_px; j += this->tile_width){
                    std::vector<std::vector<std::vector<int>>> vect(this->tile_height, std::vector<std::vector<int>>(this->tile_width, std::vector<int>(3)));

                    for(int k = 0; k < this->tile_height; k++){
                        for(int l = 0; l < this->tile_width; l++){
                            getColorAtPixel(image, j + l, ii + k, img_w, vect[k][l][0], vect[k][l][1], vect[k][l][2]);
                            
                        }
                    }

                    swap(tiles[idx], vect);
                    idx++;
                }
            }

        }



    }

    void drawTileAt(int tileId, int x, int y){

        glBegin(GL_POINTS);
        for(int i = 0; i < tile_height; i++){
            for(int j = 0; j < tile_width; j++){
                glColor3ub(tiles[tileId][i][j][0], tiles[tileId][i][j][1], tiles[tileId][i][j][2]);
                
                glVertex2i(x + j, y - i);
                
            }
        }
        glEnd();
    }

    void drawMap(int x, int y){

        for(int i = 0; i < 5; i++){
            for(int j = 0; j < 5; j++){
                int idx = tile_ids[i][j];
                int xx = x + j * tile_width;
                int yy = y - i * tile_height;
                
                drawTileAt(idx, xx, yy);
            }
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
