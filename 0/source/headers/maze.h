#ifndef MAZE_H
#define MAZE_H

#include "headers/buttons.h"
#include "headers/obstacles.h"
#include "headers/powerups.h"
#include "headers/tasks.h"
#include "opengl/global.h"
using namespace std;

//node class for cell in a maze
class mazeNode {

public:
    int x_pos;
    int y_pos;
    map<char, bool> nbrs; //walls
    mazeNode();
    mazeNode(int x, int y);
};

// node class for dsu
class dsuNode {

public:
    int x;
    int y;
    dsuNode *parent;
    dsuNode(int x_val, int y_val);
};

class Maze {

public:
    //orientation and position
    float START_X;
    float START_Y;
    float WIDTH;
    float HEIGHT;
    int rows;
    int cols;
    float box_width;
    float box_height;

    // tasks  , players , imposter , powerups  and obstacles
    vector<mazeNode *> player_nodes;
    vector<mazeNode *> imposter_nodes;
    Task *task1;
    Task *task2;
    Button *imposter_button;
    Button *powerup_button;
    vector<Powerup *> powerups;
    vector<Obstacle *> obstacles;

    //set of nodes
    vector<vector<mazeNode>> maze_grid;
    vector<vector<int>> dist;
    char imposter_move;
    vector<float> line_vertices;
    vector<float> body_vertices;
    Maze(float x, float y, float width, float height, int r, int c);

    // dsu(kruskals) functions for maze generation
    dsuNode *find_root(dsuNode &n);
    void merge_nodes(dsuNode &n1, dsuNode &n2);
    void add_edge(int orig_x, int orig_y, char direction);
    void make_maze();

    // making and drawing
    void make_lines();
    void draw_finish();
    void draw_maze();
    void draw_light();
    void draw_tasks();
    void draw_tasks_light();
    void draw_buttons();
    void draw_buttons_light();
    void draw_powerups();
    void draw_powerups_light();
    void draw_obstacles();
    void draw_obstacles_light();

    //utility functions to find node positions and vertices
    pair<float, float> find_pos(mazeNode *mNode);
    mazeNode *find_node(float x, float y);
    int vertex_dist(float x, float y);

    //updation functions to track player and imposter nodes
    vector<mazeNode *> find_player(hitbox_t player_hitbox);
    void update_player(hitbox_t player_hitbox);

    vector<mazeNode *> find_imposter(hitbox_t imposter_hitbox);
    void update_imposter(hitbox_t imposter_hitbox);

    //bfs
    void bfs();

    //checking collsions and finish conditions
    bool check_collision(hitbox_t possible_hitbox, vector<mazeNode *> activated_nodes);

    bool check_task(hitbox_t player_hitbox);
    bool check_imposter_button(hitbox_t player_hitbox);
    bool check_powerup_button(hitbox_t player_hitbox);

    bool check_powerup(hitbox_t player_hitbox);
    void spawn_powerups();

    bool check_obstacle(hitbox_t player_hitbox);
    void spawn_obstacles();

    bool check_kill();
    bool check_finish();
};

#endif