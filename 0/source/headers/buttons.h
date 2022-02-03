#ifndef BUTTONS_H
#define BUTTONS_H

#include "opengl/global.h"
using namespace std;

class Button {
public:
    Button() {}
    Button(int n_x, int n_y, float x, float y, float radius, color_t color);

    // Position on node coordinates
    glm::vec2 position;
    int node_x;
    int node_y;

    //properties
    float radius;
    color_t color;
    bool pressed;
    void create();
    void draw();
    void draw_light(int dist);
    vector<float> vertices;
    VAO *vao_object;
};

#endif