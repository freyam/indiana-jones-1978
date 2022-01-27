#ifndef IMPOSTER_H
#define IMPOSTER_H

#include "opengl/global.h"
using namespace std;

class Imposter {
public:
    Imposter() {}
    Imposter(float x, float y, float width, float height, color_t color, float speed);

    //poisiton and node coordinates
    glm::vec2 position;
    float width;
    float height;

    //properties
    color_t color;
    bool exist;
    float speed;
    void create();
    void draw();
    void draw_light(int dist);
    vector<float> vertices;
    VAO *vao_object;
};

#endif