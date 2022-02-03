#include "headers/game.h"
#include "headers/buttons.h"
#include "headers/imposter.h"
#include "headers/maze.h"
#include "headers/player.h"
#include "headers/powerups.h"
#include "headers/tasks.h"
#include "opengl/resource_manager.h"
#include "opengl/shaders.h"

Maze *game_maze;
Player *game_player;
Imposter *game_imposter;

Game::Game(unsigned int width, unsigned int height) : State(GAME_ACTIVE), Keys(), Width(width), Height(height) {
    this->total_tasks = 2;
    this->tasks_done = 0;
    this->light = true;
    this->time_remaining = 120;
}

Game::~Game() {}

void Game::Init() {

    ResourceManager::LoadShader("../source/shaders/vertex.vs", "../source/shaders/fragment.fs", nullptr, "maze");

    ResourceManager::LoadShader("../source/shaders/vertex.vs", "../source/shaders/fragment.fs", nullptr, "object");

    // Setup common projection matrix
    glm::mat4 PROJECTION = glm::ortho(SCR_LEFT / SCR_ZOOM, SCR_RIGHT / SCR_ZOOM, SCR_BOTTOM / SCR_ZOOM, SCR_TOP / SCR_ZOOM, 0.1f, 100.0f);

    // ONLY FOR MAZE - UNIT MODEL MATRIX
    glm::mat4 MAZE_MODEL = glm::mat4(1.0f);
    ResourceManager::GetShader("maze").SetMatrix4("model", MAZE_MODEL, true);

    //Set projection matrix
    ResourceManager::GetShader("maze").SetMatrix4("projection", PROJECTION, true);
    ResourceManager::GetShader("object").SetMatrix4("projection", PROJECTION, true);

    // make the maze
    game_maze = new Maze(SCR_LEFT + 50.0f, SCR_TOP - 50.0f, (SCR_RIGHT - SCR_LEFT) - 100.0f, (SCR_TOP - SCR_BOTTOM) - 100.0f, 15, 15);

    game_maze->make_maze();
    game_maze->make_lines();

    //make the player
    float player_width = game_maze->box_width * 0.35;
    float player_height = game_maze->box_height * 0.55;
    pair<float, float> starting_pos = game_maze->find_pos(game_maze->player_nodes[0]);
    game_player = new Player(starting_pos.first, starting_pos.second, player_width, player_height, COLOR_BLUE, PLAYER_SPEED);
    game_player->create();

    //make the imposter
    float imposter_width = game_maze->box_width * 0.35;
    float imposter_height = game_maze->box_height * 0.55;
    pair<float, float> imposter_start = game_maze->find_pos(game_maze->imposter_nodes[0]);
    game_imposter = new Imposter(imposter_start.first, imposter_start.second, imposter_width, imposter_height, COLOR_RED, IMPOSTER_SPEED);
    game_imposter->create();

    game_imposter = new Imposter(imposter_start.first, imposter_start.second, imposter_width, imposter_height, COLOR_RED, IMPOSTER_SPEED);
    game_imposter->create();

    //making objects
    bool hash[game_maze->rows][game_maze->cols];
    for (int i = 0; i < game_maze->rows; i++) {
        for (int j = 0; j < game_maze->cols; j++)
            hash[i][j] = false;
    }

    hash[game_maze->player_nodes[0]->y_pos][game_maze->player_nodes[0]->x_pos] = true;
    hash[game_maze->imposter_nodes[0]->y_pos][game_maze->imposter_nodes[0]->x_pos] = true;
    hash[game_maze->rows - 1][game_maze->cols - 1] = true;
    int r, c;
    srand((unsigned)time(0));

    // 2 tasks
    while (1) {
        r = rand() % game_maze->rows;
        c = rand() % game_maze->cols;
        if (!hash[r][c]) {
            hash[r][c] = true;
            break;
        }
    }
    pair<float, float> pos = game_maze->find_pos(&(game_maze->maze_grid[r][c]));
    float width = game_maze->box_width * 0.95;
    float height = game_maze->box_height * 0.95;
    game_maze->task1 = new Task(c, r, pos.first, pos.second, width, height, COLOR_YELLOW);
    game_maze->task1->create();

    while (1) {
        r = rand() % game_maze->rows;
        c = rand() % game_maze->cols;
        if (!hash[r][c]) {
            hash[r][c] = true;
            break;
        }
    }
    pos = game_maze->find_pos(&(game_maze->maze_grid[r][c]));
    width = game_maze->box_width * 0.95;
    height = game_maze->box_height * 0.95;
    game_maze->task2 = new Task(c, r, pos.first, pos.second, width, height, COLOR_YELLOW);
    game_maze->task2->create();

    //2 buttons
    while (1) {
        r = (game_maze->rows - 4) + rand() % 3;
        c = rand() % 4;
        if (!hash[r][c]) {
            hash[r][c] = true;
            break;
        }
    }
    pos = game_maze->find_pos(&(game_maze->maze_grid[r][c]));
    width = game_maze->box_width * 0.5;
    height = game_maze->box_height * 0.5;
    float rad;
    if (width < height)
        rad = width;
    else
        rad = height;
    game_maze->imposter_button = new Button(c, r, pos.first, pos.second, rad, COLOR_RED);
    game_maze->imposter_button->create();

    while (1) {
        r = rand() % game_maze->rows;
        c = rand() % game_maze->cols;
        if (!hash[r][c]) {
            hash[r][c] = true;
            break;
        }
    }
    pos = game_maze->find_pos(&(game_maze->maze_grid[r][c]));
    width = game_maze->box_width * 0.5;
    height = game_maze->box_height * 0.5;
    if (width < height)
        rad = width;
    else
        rad = height;
    game_maze->powerup_button = new Button(c, r, pos.first, pos.second, rad, COLOR_BLUE);
    game_maze->powerup_button->create();

    // 3 powerups
    for (int i = 0; i < 3; i++) {
        Powerup *pUp;
        while (1) {
            r = rand() % game_maze->rows;
            c = rand() % game_maze->cols;
            if (!hash[r][c]) {
                hash[r][c] = true;
                break;
            }
        }
        pos = game_maze->find_pos(&(game_maze->maze_grid[r][c]));
        width = game_maze->box_width * 0.65;
        height = game_maze->box_height * 0.65;
        pUp = new Powerup(c, r, pos.first, pos.second, width, height, COLOR_GREEN);
        game_maze->powerups.push_back(pUp);
        pUp->create();
    }

    //3 obstacles
    for (int i = 0; i < 3; i++) {
        Obstacle *obs;
        while (1) {
            r = rand() % game_maze->rows;
            c = rand() % game_maze->cols;
            if (!hash[r][c]) {
                hash[r][c] = true;
                break;
            }
        }
        pos = game_maze->find_pos(&(game_maze->maze_grid[r][c]));
        width = game_maze->box_width * 0.65;
        height = game_maze->box_height * 0.65;
        obs = new Obstacle(c, r, pos.first, pos.second, width, height, COLOR_BROWN);
        game_maze->obstacles.push_back(obs);
        obs->create();
    }

    this->prev_time = glfwGetTime();
}

// passing of one second
void Game::tick() {
    if ((glfwGetTime() - this->prev_time) >= 1.0f) {
        this->time_remaining -= 1;
        this->prev_time = glfwGetTime();

        //increase points if in dark
        if (!this->light)
            game_player->health += 1;
    }
}

void Game::Update(float dt) {
    if (this->State == GAME_ACTIVE) {

        // run bfs to update distances from player
        game_maze->bfs();

        // Move imposter according to direction returned by bfs
        float velocity = dt * game_imposter->speed;

        if (game_imposter->exist) {
            if (game_maze->imposter_move == 'N')
                game_imposter->position.y += velocity;

            else if (game_maze->imposter_move == 'E')
                game_imposter->position.x += velocity;

            else if (game_maze->imposter_move == 'S')
                game_imposter->position.y -= velocity;

            else if (game_maze->imposter_move == 'W')
                game_imposter->position.x -= velocity;

            hitbox_t imposter_hitbox = get_hitbox(game_imposter->position.x, game_imposter->position.y, game_imposter->width, game_imposter->height);
            game_maze->update_imposter(imposter_hitbox);
        }

        // check for any cllsions , powerups collections , tasks completion , lose conditions , finish conditions
        if (game_maze->check_task(get_hitbox(game_player->position.x, game_player->position.y, game_player->width, game_player->height))) {
            game_player->health += 5;
            this->tasks_done += 1;
        }

        if (game_maze->check_powerup(get_hitbox(game_player->position.x, game_player->position.y, game_player->width, game_player->height))) {
            game_player->health += 2;
        }

        if (game_maze->check_obstacle(get_hitbox(game_player->position.x, game_player->position.y, game_player->width, game_player->height))) {
            if (game_player->isHurting == false) {
                game_player->isHurting = true;
                game_player->health -= 3;
                game_player->prev_hurt = glfwGetTime();
            }
        } else
            game_player->isHurting = false;

        if (game_player->isHurting) {
            if ((glfwGetTime() - game_player->prev_hurt) >= 0.6) {
                game_player->health -= 3;
                game_player->prev_hurt = glfwGetTime();
            }
        }

        if (game_maze->check_imposter_button(get_hitbox(game_player->position.x, game_player->position.y, game_player->width, game_player->height))) {
            game_player->health += 1;
            game_imposter->exist = false;
            game_maze->imposter_nodes.clear();
            // cout << "died" << endl;
        }

        if (game_maze->check_powerup_button(get_hitbox(game_player->position.x, game_player->position.y, game_player->width, game_player->height))) {
            game_player->health += 1;
            game_maze->spawn_powerups();
            game_maze->spawn_obstacles();
        }

        this->player_health = game_player->health;

        if (game_imposter->exist)
            if (game_maze->check_kill())
                this->State = GAME_LOST;

        if (this->tasks_done == 2) {
            if (game_maze->check_finish())
                this->State = GAME_WON;
        }

        if (this->player_health <= 0)
            this->State = GAME_LOST;

        this->tick();
        if (this->time_remaining <= 0)
            this->State = GAME_LOST;
    }
}

void Game::ProcessInput(float dt) {
    if (this->State == GAME_ACTIVE) {
        // process player movement
        float velocity = dt * game_player->speed;
        hitbox_t player_hitbox = get_hitbox(game_player->position.x, game_player->position.y, game_player->width, game_player->height);
        hitbox_t possible_hitbox;
        glm::vec2 new_position;

        if (Keys[GLFW_KEY_A]) {
            new_position.x = game_player->position.x - velocity;
            new_position.y = game_player->position.y;
            possible_hitbox = get_hitbox(new_position.x, new_position.y, game_player->width, game_player->height);

            if (!(game_maze->check_collision(possible_hitbox, game_maze->find_player(possible_hitbox)))) {
                game_player->position.x -= velocity;
                player_hitbox = get_hitbox(game_player->position.x, game_player->position.y, game_player->width, game_player->height);
                game_maze->update_player(player_hitbox);
            }
        }
        if (Keys[GLFW_KEY_D]) {
            new_position.x = game_player->position.x + velocity;
            new_position.y = game_player->position.y;
            possible_hitbox = get_hitbox(new_position.x, new_position.y, game_player->width, game_player->height);

            if (!(game_maze->check_collision(possible_hitbox, game_maze->find_player(possible_hitbox)))) {
                game_player->position.x += velocity;
                player_hitbox = get_hitbox(game_player->position.x, game_player->position.y, game_player->width, game_player->height);
                game_maze->update_player(player_hitbox);
            }
        }
        if (Keys[GLFW_KEY_S]) {
            new_position.x = game_player->position.x;
            new_position.y = game_player->position.y - velocity;
            possible_hitbox = get_hitbox(new_position.x, new_position.y, game_player->width, game_player->height);

            if (!(game_maze->check_collision(possible_hitbox, game_maze->find_player(possible_hitbox)))) {
                game_player->position.y -= velocity;
                player_hitbox = get_hitbox(game_player->position.x, game_player->position.y, game_player->width, game_player->height);
                game_maze->update_player(player_hitbox);
            }
        }
        if (Keys[GLFW_KEY_W]) {
            new_position.x = game_player->position.x;
            new_position.y = game_player->position.y + velocity;
            possible_hitbox = get_hitbox(new_position.x, new_position.y, game_player->width, game_player->height);

            if (!(game_maze->check_collision(possible_hitbox, game_maze->find_player(possible_hitbox)))) {
                game_player->position.y += velocity;
                player_hitbox = get_hitbox(game_player->position.x, game_player->position.y, game_player->width, game_player->height);
                game_maze->update_player(player_hitbox);
            }
        }
    }
}

void Game::Render() {
    if (this->State == GAME_ACTIVE) {

        // setting up the view matrix to look at player
        glm::mat4 VIEW = glm::lookAt(glm::vec3(game_player->position, 3.0f), glm::vec3(game_player->position, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        ResourceManager::GetShader("maze").SetMatrix4("view", VIEW, true);
        ResourceManager::GetShader("object").SetMatrix4("view", VIEW, true);

        // render maze
        ResourceManager::GetShader("maze").Use();
        if (this->light)
            game_maze->draw_maze();
        else
            game_maze->draw_light();

        // render objects
        if (this->light)
            game_maze->draw_tasks();
        else
            game_maze->draw_tasks_light();

        if (this->light)
            game_maze->draw_buttons();
        else
            game_maze->draw_buttons_light();

        if (this->light) {
            game_maze->draw_powerups();
            game_maze->draw_obstacles();
        } else {
            game_maze->draw_powerups_light();
            game_maze->draw_obstacles_light();
        }

        //render player
        if (game_player->isHurting)
            game_player->color = COLOR_PURPLE;
        else
            game_player->color = COLOR_BLUE;
        game_player->draw();

        //render imposter
        if (game_imposter->exist) {
            if (this->light)
                game_imposter->draw();
            else {
                int d;
                if (game_maze->imposter_nodes.size() == 1)
                    d = game_maze->dist[game_maze->imposter_nodes[0]->y_pos][game_maze->imposter_nodes[0]->x_pos];
                else {
                    if (game_maze->dist[game_maze->imposter_nodes[0]->y_pos][game_maze->imposter_nodes[0]->x_pos] < game_maze->dist[game_maze->imposter_nodes[1]->y_pos][game_maze->imposter_nodes[1]->x_pos])
                        d = game_maze->dist[game_maze->imposter_nodes[0]->y_pos][game_maze->imposter_nodes[0]->x_pos];
                    else
                        d = game_maze->dist[game_maze->imposter_nodes[1]->y_pos][game_maze->imposter_nodes[1]->x_pos];
                }

                game_imposter->draw_light(d);
            }
        }
    }
}
