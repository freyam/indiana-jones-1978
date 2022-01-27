#ifndef GAME_H
#define GAME_H

#include "glad/glad.h"
#include <GLFW/glfw3.h>

enum GameState {
    GAME_ACTIVE,
    GAME_WON,
    GAME_LOST
};

class Game {
public:
    // game state
    GameState State;

    //Hud values
    int tasks_done;
    int player_health;
    bool light;
    int total_tasks;

    int time_remaining;
    float prev_time;
    void tick();

    // keys for input
    bool Keys[1024];

    // dimensions
    unsigned int Width;
    unsigned int Height;

    Game(unsigned int width, unsigned int height);
    ~Game();

    // initialize state and all objects
    void Init();

    // process input
    void ProcessInput(float dt);

    // update states
    void Update(float dt);

    // render
    void Render();
};

#endif