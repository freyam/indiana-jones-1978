#ifndef HEAD_H
#define HEAD_H

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLT_IMPLEMENTATION
#include "utilityHeaders/gltext.h"

#include <bits/stdc++.h>
#include <iostream>

extern float SCR_WIDTH;
extern float SCR_HEIGHT;
extern float SCR_LEFT;
extern float SCR_RIGHT;
extern float SCR_TOP;
extern float SCR_BOTTOM;
extern float SCR_ZOOM;

extern float PLAYER_SPEED;
extern float IMPOSTER_SPEED;

extern int INF;

struct VAO {
    GLuint VertexArrayID;
    GLuint VertexBuffer;
    GLuint ColorBuffer;

    GLenum PrimitiveMode;
    GLenum FillMode;
    int NumVertices;
};
typedef struct VAO VAO;

struct color_t {
    float r;
    float g;
    float b;
};

struct hitbox_t {
    float center_x;
    float center_y;
    float width;
    float height;
};

hitbox_t get_hitbox(float x, float y, float width, float height);

bool detect_collision(hitbox_t a, hitbox_t b);

extern color_t COLOR_BLUE;
extern color_t COLOR_RED;
extern color_t COLOR_GREEN;
extern color_t COLOR_YELLOW;
extern color_t COLOR_BROWN;
extern color_t COLOR_PURPLE;
extern color_t COLOR_GREY;
extern color_t COLOR_BLACK;
extern color_t COLOR_WHITE;
extern color_t COLOR_DARKWHITE;

#endif