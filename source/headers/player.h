#ifndef PLAYER_H
#define PLAYER_H

#include "opengl/global.h"
using namespace std;

class Player {
public:
    Player() {}
    Player(float x, float y, float width, float height, color_t color, float speed);

    //poisiton
    glm::vec2 position;

    // properties
    float width;
    float height;
    color_t color;
    int health;
    float speed;
    bool isHurting; //if player is on an obstacle
    float prev_hurt;
    void create();
    void draw();
    vector<float> vertices;
    VAO *vao_object;
};

#endif