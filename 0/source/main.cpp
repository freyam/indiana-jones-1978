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

Camera camera(glm::vec3(0.0, 0.0, 4.0));
float SCR_WIDTH = 1000.0f;
float SCR_HEIGHT = 1000.0f;
float SCR_LEFT = (-SCR_WIDTH / 2);
float SCR_RIGHT = (SCR_WIDTH / 2);
float SCR_BOTTOM = (-SCR_HEIGHT / 2);
float SCR_TOP = (SCR_HEIGHT / 2);
float SCR_ZOOM = 1.5f;

float PLAYER_SPEED = 80.0f;
float IMPOSTER_SPEED = 40.0f;

int INF = 99999;

Game IJ(SCR_WIDTH, SCR_HEIGHT);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (key == GLFW_KEY_L && action == GLFW_PRESS)
        IJ.light = 1 - IJ.light;

    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)
            IJ.Keys[key] = true;
        else if (action == GLFW_RELEASE)
            IJ.Keys[key] = false;
    }
}

int main(int argc, char **argv) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Indiana Jones 1978 Limited Edition", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, processInput);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    if (!gltInit()) {
        cout << "Failed to initialize glText" << endl;
        glfwTerminate();
        return 1;
    }

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    IJ.Init();

    GLTtext *health_text = gltCreateText();
    GLTtext *tasks_text = gltCreateText();
    GLTtext *light_text = gltCreateText();
    GLTtext *time_text = gltCreateText();

    char current_health[50];
    char current_tasks[50];
    char current_time[50];

    while (true) {
        if (glfwWindowShouldClose(window))
            break;

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        IJ.ProcessInput(deltaTime);

        IJ.Update(deltaTime);

        sprintf(current_health, "Health: %d", IJ.player_health);
        sprintf(current_tasks, "Tasks done: %d/%d", IJ.tasks_done, IJ.total_tasks);
        sprintf(current_time, "Time left: %d", IJ.time_remaining);

        gltSetText(health_text, current_health);
        gltSetText(tasks_text, current_tasks);

        if (IJ.light)
            gltSetText(light_text, "Lights ON");
        else
            gltSetText(light_text, "Lights OFF");

        gltSetText(time_text, current_time);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (IJ.State == GAME_LOST || IJ.State == GAME_WON) {
            if (IJ.State == GAME_WON) {
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
            } else {
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
