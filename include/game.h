#ifndef GAME_H
#define GAME_H

#include "game_level.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

enum GameState {
    GAME_INTRO,
    GAME_ACTIVE,
    GAME_LOSE,
    GAME_WIN
};
const glm::vec2 PLAYER_SIZE(32.0f, 32.0f);
const float PLAYER_VELOCITY(300.0f);
const float MONSTER_VELOCITY(250.0f);

enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

typedef std::tuple<bool, Direction, glm::vec2> Collision;

class Game {
public:
    GameState State;
    bool Keys[1024];
    unsigned int Width, Height;
    GameLevel ARENA;
    unsigned int Level;
    unsigned int nPriests;
    unsigned int WALL_FACTOR;
    unsigned int COIN_FACTOR;
    unsigned int Points;

    glm::vec2 playerPos;
    bool light;

    Game(unsigned int width, unsigned int height);
    ~Game();

    void Init();
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();

    void createArena();

    void PriestMovement(GameObject *Priest, float dt);

    void collisionsProofingIndianaJones(float dt);
    void collisionsProofingPriest(GameObject *Priest, float dt);

    bool levelCompleted();
    void goToNextLevel();

    void ResetLevel();
    void ResetPlayer();
    void Reset();
};

#endif