#include "game.h"
#include "game_level.h"
#include "game_object.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "text_renderer.h"

#include <bits/stdc++.h>

void randomMovemenetPriest(GameObject *Priest, float dt);
Collision checkCollision(GameObject &one, GameObject &two);
Direction VectorDirection(glm::vec2 target);

SpriteRenderer *Renderer;
GameObject *IndianaJones;
const unsigned int MAX_PRIESTS = 10;
GameObject *Priests[MAX_PRIESTS];

TextRenderer *Text;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_INTRO), Keys(), Width(width), Height(height) {
}

Game::~Game() {
    delete Renderer;
    delete IndianaJones;
    for (int i = 0; i < this->nPriests; i++) {
        delete Priests[i];
    }
}

void Game::Init() {
    ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.frag", nullptr, "sprite");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
                                      static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));

    ResourceManager::LoadTexture("textures/intro.png", true, "intro");
    ResourceManager::LoadTexture("textures/background.png", true, "background");
    ResourceManager::LoadTexture("textures/coin.png", true, "coin");
    ResourceManager::LoadTexture("textures/indiana-jones.png", true, "indiana-jones");
    ResourceManager::LoadTexture("textures/priest.png", true, "priest");
    ResourceManager::LoadTexture("textures/wall.png", true, "wall");
    ResourceManager::LoadTexture("textures/win.png", true, "win");
    ResourceManager::LoadTexture("textures/lose.png", true, "lose");

    this->Level = 0;
    this->nPriests = 2;
    this->WALL_FACTOR = 6;
    this->COIN_FACTOR = 7;

    GameLevel gameLevel;
    createArena();
    gameLevel.Load("arena/tiles.txt", this->Width, this->Height);
    this->ARENA = gameLevel;

    glm::vec2 playerPos = glm::vec2(0, this->Height / 2.0f - PLAYER_SIZE.y / 2.0f + 14.5f);
    IndianaJones = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("indiana-jones"));

    for (int i = 0; i < MAX_PRIESTS; ++i) {
        int deltaX = rand() % 400;
        int deltaY = rand() % 300;
        glm::vec2 priestPosition = glm::vec2(this->Width - PLAYER_SIZE.x - deltaX, this->Height - PLAYER_SIZE.y - deltaY);
        Priests[i] = new GameObject(priestPosition, PLAYER_SIZE, ResourceManager::GetTexture("priest"));
    }

    this->light = false;
    this->timeElapsed = 0;

    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("src/fonts/OCRAEXT.TTF", 24);
}

void Game::Update(float dt) {
    this->playerPos = IndianaJones->Position;
    this->timeElapsed += (dt * 1000);

    this->collisionsProofingIndianaJones(dt);
    for (int i = 0; i < this->nPriests; ++i) {
        PriestMovement(Priests[i], dt);
        this->collisionsProofingPriest(Priests[i], dt);
    }

    if (levelCompleted())
        goToNextLevel();
}

void Game::createArena() {
    srand(time(NULL));
    unsigned int arena[10][20] = {0};

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 20; j++) {
            if ((i == 4 && j == 0) || (i == 4 && j == 19))
                arena[i][j] = 0;
            else if (i == 0 || i == 9 || j == 0 || j == 19)
                arena[i][j] = 1;
        }
    }

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 20; j++) {
            if (i < 2 || i > 7 || j < 2 || j > 17)
                continue;
            if (arena[i - 1][j] == 1 || arena[i + 1][j] == 1 || arena[i][j - 1] == 1 || arena[i][j + 1] == 1)
                arena[i][j] = 0;
            else if (arena[i - 1][j - 1] == 1 || arena[i + 1][j + 1] == 1 || arena[i - 1][j + 1] == 1 || arena[i + 1][j - 1] == 1)
                arena[i][j] = 0;
            else if (arena[i][j] == 0 && rand() % this->COIN_FACTOR == 0)
                arena[i][j] = 2;
            else if (arena[i][j] == 0 && rand() % this->WALL_FACTOR == 0)
                arena[i][j] = 1;
        }
    }

    std::ofstream outfile("arena/tiles.txt");
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 20; j++) {
            outfile << arena[i][j] << " ";
        }
        outfile << std::endl;
    }
}
void Game::ProcessInput(float dt) {
    if (this->State == GAME_ACTIVE) {
        float velocity = PLAYER_VELOCITY * dt;
        if (this->Keys[GLFW_KEY_W] || this->Keys[GLFW_KEY_UP]) {
            if (IndianaJones->Position.y >= 0.0f)
                IndianaJones->Position.y -= velocity;
        }
        if (this->Keys[GLFW_KEY_A] || this->Keys[GLFW_KEY_LEFT]) {
            if (IndianaJones->Position.x >= 0.0f)
                IndianaJones->Position.x -= velocity;
        }
        if (this->Keys[GLFW_KEY_S] || this->Keys[GLFW_KEY_DOWN]) {
            if (IndianaJones->Position.y <= this->Height - IndianaJones->Size.y)
                IndianaJones->Position.y += velocity;
        }
        if (this->Keys[GLFW_KEY_D] || this->Keys[GLFW_KEY_RIGHT]) {
            if (IndianaJones->Position.x <= this->Width - IndianaJones->Size.x)
                IndianaJones->Position.x += velocity;
        }
    }
}

void Game::Render() {
    if (this->State == GAME_ACTIVE) {
        Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f, glm::vec3(1.0f), this->playerPos, this->light);
        ARENA.Draw(*Renderer, this->playerPos, this->light);
        IndianaJones->Draw(*Renderer, this->playerPos, this->light);
        for (int i = 0; i < this->nPriests; ++i) {
            Priests[i]->Draw(*Renderer, this->playerPos, this->light);
        }

        std::stringstream ss;
        ss << "Level: " << this->Level + 1 << "  "
           << "Priests: " << this->nPriests << "  "
           << "Time: " << this->timeElapsed / 1000 << "  "
           << "Score: " << this->Points;

        Text->RenderText(ss.str(), 5.0f, 5.0f, 1.0f);

    } else if (this->State == GAME_LOSE) {
        this->light = false;
        Renderer->DrawSprite(ResourceManager::GetTexture("lose"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f, glm::vec3(1.0f), this->playerPos, this->light);
    } else if (this->State == GAME_WIN) {
        this->light = false;
        Renderer->DrawSprite(ResourceManager::GetTexture("win"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f, glm::vec3(1.0f), this->playerPos, this->light);
    } else {
        // Renderer->DrawSprite(ResourceManager::GetTexture("intro"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f, glm::vec3(1.0f), this->playerPos, this->light);
        this->State = GAME_ACTIVE;
    }
}

void Game::PriestMovement(GameObject *Priest, float dt) {
    float velocity = MONSTER_VELOCITY * dt * 0.5f;
    if (IndianaJones->Position.x < Priest->Position.x) {
        Priest->Position.x -= velocity;
    } else if (IndianaJones->Position.x > Priest->Position.x) {
        Priest->Position.x += velocity;
    }
    if (IndianaJones->Position.y < Priest->Position.y) {
        Priest->Position.y -= velocity;
    } else if (IndianaJones->Position.y > Priest->Position.y) {
        Priest->Position.y += velocity;
    }
}

void Game::collisionsProofingIndianaJones(float dt) {
    for (GameObject &box : ARENA.Bricks) {
        if (!box.Destroyed) {
            Collision collision = checkCollision(*IndianaJones, box);
            if (std::get<0>(collision)) {
                if (!box.IsSolid) {
                    box.Destroyed = true;
                    if (this->light)
                        this->Points += 10;
                    else
                        this->Points += 5;
                }

                Direction dir = std::get<1>(collision);
                glm::vec2 diff_vector = std::get<2>(collision);
                if (dir == LEFT || dir == RIGHT || dir == UP || dir == DOWN) {
                    if (dir == LEFT)
                        IndianaJones->Position.x -= PLAYER_VELOCITY * dt;
                    else if (dir == RIGHT)
                        IndianaJones->Position.x += PLAYER_VELOCITY * dt;
                    else if (dir == UP)
                        IndianaJones->Position.y += PLAYER_VELOCITY * dt;
                    else
                        IndianaJones->Position.y -= PLAYER_VELOCITY * dt;
                }
            }

            for (int i = 0; i < this->nPriests; ++i) {
                Collision collision = checkCollision(*IndianaJones, *Priests[i]);
                if (std::get<0>(collision)) {
                    this->State = GAME_LOSE;
                }
            }
        }
    }
}

void Game::collisionsProofingPriest(GameObject *PriestO, float dt) {
    for (GameObject &box : ARENA.Bricks) {
        if (!box.Destroyed) {
            Collision collision = checkCollision(*PriestO, box);
            if (std::get<0>(collision)) {
                if (!box.IsSolid) {
                    continue;
                }

                Direction dir = std::get<1>(collision);
                glm::vec2 diff_vector = std::get<2>(collision);
                if (dir == LEFT || dir == RIGHT || dir == UP || dir == DOWN) {
                    if (dir == LEFT)
                        PriestO->Position.x -= PLAYER_VELOCITY * dt;
                    else if (dir == RIGHT)
                        PriestO->Position.x += PLAYER_VELOCITY * dt;
                    else if (dir == UP)
                        PriestO->Position.y += PLAYER_VELOCITY * dt;
                    else
                        PriestO->Position.y -= PLAYER_VELOCITY * dt;
                }
            }
        }
    }
}

Direction VectorDirection(glm::vec2 target) {
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(0.0f, -1.0f),
        glm::vec2(-1.0f, 0.0f)};
    float max = 0.0f;
    unsigned int best_match = -1;
    for (unsigned int i = 0; i < 4; i++) {
        float dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max) {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}

Collision checkCollision(GameObject &one, GameObject &two) {
    glm::vec2 difference = one.Position - two.Position;
    if (one.Position.x + one.Size.x >= two.Position.x && one.Position.x <= two.Position.x + two.Size.x) {
        if (one.Position.y + one.Size.y >= two.Position.y && one.Position.y <= two.Position.y + two.Size.y) {
            return std::make_tuple(true, VectorDirection(difference), difference);
            ;
        }
    }
    return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
}

bool Game::levelCompleted() {
    return (IndianaJones->Position.x >= this->Width - PLAYER_SIZE.x);
}

void Game::goToNextLevel() {
    this->Level++;

    if (this->Level == 1) {
        this->nPriests = 3;
        this->WALL_FACTOR = 4;
        this->COIN_FACTOR = 5;
    }

    if (this->Level == 2) {
        this->nPriests = 5;
        this->WALL_FACTOR = 2;
        this->COIN_FACTOR = 2;
    }

    if (this->Level == 3) {
        this->State = GAME_WIN;
        return;
    }

    createArena();
    ARENA.Load("arena/tiles.txt", this->Width, this->Height);

    IndianaJones->Position = glm::vec2(0, this->Height / 2.0f - PLAYER_SIZE.y / 2.0f + 14.5f);
}
