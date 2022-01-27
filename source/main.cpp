#include "headers/game.h"
#include "opengl/camera.h"
#include "opengl/global.h"
#include "opengl/resource_manager.h"
#include "opengl/shaders.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window, int key, int scancode, int action, int mode);

void draw_win();
void draw_lose();

// declaring the camera object
Camera camera(glm::vec3(0.0, 0.0, 3.0));
float SCR_WIDTH = 800.0f;
float SCR_HEIGHT = 800.0f;
float SCR_LEFT = (-SCR_WIDTH / 2);
float SCR_RIGHT = (SCR_WIDTH / 2);
float SCR_BOTTOM = (-SCR_HEIGHT / 2);
float SCR_TOP = (SCR_HEIGHT / 2);
float SCR_ZOOM = 1.5f;

float PLAYER_SPEED = 80.0f;
float IMPOSTER_SPEED = 40.0f;

int INF = 99999;

// game object
Game IJ(SCR_WIDTH, SCR_HEIGHT);

//frames and times
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main(int argc, char **argv) {

    // initialize and set window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create a window object
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Indiana Jones 1978 Limited Edition", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }

    // make this window as the current state (context)
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, processInput);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    if (!gltInit()) {
        fprintf(stderr, "Failed to initialize glText\n");
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // glEnable(GL_DEPTH_TEST);

    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //Initialize Game
    IJ.Init();
    GLTtext *health_text = gltCreateText();
    GLTtext *tasks_text = gltCreateText();
    GLTtext *light_text = gltCreateText();
    GLTtext *time_text = gltCreateText();
    char health_str[50];
    char tasks_str[50];
    char time_str[50];
    // render loop
    while (!glfwWindowShouldClose(window)) {
        // set times
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // process user input
        IJ.ProcessInput(deltaTime);

        //update state
        IJ.Update(deltaTime);

        // hud text update
        sprintf(health_str, "Health: %d", IJ.player_health);
        sprintf(tasks_str, "Tasks done: %d/%d", IJ.tasks_done, IJ.total_tasks);
        sprintf(time_str, "Time left: %d", IJ.time_remaining);

        gltSetText(health_text, health_str);
        gltSetText(tasks_text, tasks_str);

        if (IJ.light)
            gltSetText(light_text, "Light: ON");
        else
            gltSetText(light_text, "Light: OFF");

        gltSetText(time_text, time_str);

        // set background color and clear screen and depth buffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        if (IJ.State == GAME_LOST || IJ.State == GAME_WON) {
            if (IJ.State == GAME_WON)
                draw_win();
            else
                draw_lose();
        } else {
            IJ.Render();
            gltBeginDraw();

            if (IJ.player_health >= 5)
                gltColor(COLOR_BLUE.r, COLOR_BLUE.g, COLOR_BLUE.b, 1.0f);
            else
                gltColor(COLOR_RED.r, COLOR_RED.g, COLOR_RED.b, 1.0f);
            gltDrawText2DAligned(health_text, 0.0f, 15.0f, 1.5f, GLT_LEFT, GLT_CENTER);

            if (IJ.tasks_done < 2)
                gltColor(COLOR_YELLOW.r, COLOR_YELLOW.g, COLOR_YELLOW.b, 1.0f);
            else
                gltColor(COLOR_GREEN.r, COLOR_GREEN.g, COLOR_GREEN.b, 1.0f);
            gltDrawText2DAligned(tasks_text, 0.0f, 45.0f, 1.5f, GLT_LEFT, GLT_CENTER);

            gltColor(COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b, 1.0f);
            gltDrawText2DAligned(light_text, 0.0f, 75.0f, 1.5f, GLT_LEFT, GLT_CENTER);

            if (IJ.time_remaining >= 15)
                gltColor(COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b, 1.0f);
            else
                gltColor(COLOR_RED.r, COLOR_RED.g, COLOR_RED.b, 1.0f);
            gltDrawText2DAligned(time_text, 0.0f, 105.0f, 1.5f, GLT_LEFT, GLT_CENTER);

            gltEndDraw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ResourceManager::Clear();
    glfwTerminate();
    return 0;
}

//set viewport (and adjust even on resizing)
//this function is called initially and then on every resize
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

//process keys pressed
void processInput(GLFWwindow *window, int key, int scancode, int action, int mode) {
    // when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (key == GLFW_KEY_L && action == GLFW_PRESS)
        IJ.light = !IJ.light;
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)
            IJ.Keys[key] = true;
        else if (action == GLFW_RELEASE)
            IJ.Keys[key] = false;
    }
}

hitbox_t get_hitbox(float x, float y, float width, float height) {
    hitbox_t hitbox;
    hitbox.center_x = x;
    hitbox.center_y = y;
    hitbox.width = width;
    hitbox.height = height;

    return hitbox;
}

bool detect_collision(hitbox_t a, hitbox_t b) {
    return (abs(a.center_x - b.center_x) * 2 < (a.width + b.width)) &&
           (abs(a.center_y - b.center_y) * 2 < (a.height + b.height));
}

void draw_lose() {
    GLTtext *splash_text = gltCreateText();
    GLTtext *options_text = gltCreateText();
    gltSetText(splash_text, "YOU DIED!");
    gltSetText(options_text, "Quit: Esc");

    gltBeginDraw();

    gltColor(COLOR_RED.r, COLOR_RED.g, COLOR_RED.b, 1.0f);

    gltDrawText2D(splash_text, SCR_WIDTH / 2 - 150.0f, SCR_HEIGHT / 2, 4.0f);

    gltColor(COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b, 1.0f);

    gltDrawText2D(options_text, SCR_WIDTH / 2 - 75.0f, SCR_HEIGHT / 2 + 100.0f, 2.0f);

    gltEndDraw();
}

void draw_win() {
    GLTtext *splash_text = gltCreateText();
    GLTtext *options_text = gltCreateText();
    gltSetText(splash_text, "YOU ESCAPED!");
    gltSetText(options_text, "Quit: Esc");

    gltBeginDraw();

    gltColor(COLOR_BLUE.r, COLOR_BLUE.g, COLOR_BLUE.b, 1.0f);

    gltDrawText2D(splash_text, SCR_WIDTH / 2 - 150.0f, SCR_HEIGHT / 2, 4.0f);

    gltColor(COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b, 1.0f);

    gltDrawText2D(options_text, SCR_WIDTH / 2 - 75.0f, SCR_HEIGHT / 2 + 100.0f, 2.0f);

    gltEndDraw();
}