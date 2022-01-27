#include "headers/buttons.h"
#include "opengl/resource_manager.h"
using namespace std;

Button::Button(int n_x, int n_y, float x, float y, float radius, color_t color) {
    node_x = n_x;
    node_y = n_y;
    position.x = x;
    position.y = y;
    pressed = false;
    this->radius = radius;
    this->color = color;
    // cout << this->color.r << " " << this->color.g << " " << this->color.b << endl;

    //generating circle vertices
    int numVerts = 362;
    float dPi = M_PI * 2.0f;

    float circleX[numVerts];
    float circleY[numVerts];
    float circleZ[numVerts];

    circleX[0] = 0.0f;
    circleY[0] = 0.0f;
    circleZ[0] = 0.0f;

    for (int i = 1; i < numVerts; i++) {
        circleX[i] = 0.5 * cos(i * dPi / 360);
        circleY[i] = 0.5 * sin(i * dPi / 360);
        circleZ[i] = 0.0f;
    }

    float verts[3 * numVerts];

    for (int i = 0; i < numVerts; i++) {
        verts[i * 3] = circleX[i];
        verts[i * 3 + 1] = circleY[i];
        verts[i * 3 + 2] = circleZ[i];
    }

    for (auto v : verts)
        vertices.push_back(v);
}

// function to create a vao object
void Button::create() {
    struct VAO *vao = new struct VAO;
    vao->PrimitiveMode = GL_TRIANGLE_FAN;
    vao->NumVertices = 362;

    glGenVertexArrays(1, &(vao->VertexArrayID));

    glGenBuffers(1, &(vao->VertexBuffer));
    glGenBuffers(1, &(vao->ColorBuffer));

    glBindVertexArray(vao->VertexArrayID);

    glBindBuffer(GL_ARRAY_BUFFER, vao->VertexBuffer);
    float vertex_buffer[vertices.size()];
    for (int i = 0; i < vertices.size(); i++)
        vertex_buffer[i] = vertices[i];

    glBufferData(GL_ARRAY_BUFFER, 3 * vao->NumVertices * sizeof(float), vertex_buffer, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    vao_object = vao;
}

// function to draw from the vao
void Button::draw() {

    glBindVertexArray(vao_object->VertexArrayID);
    glBindBuffer(GL_ARRAY_BUFFER, vao_object->ColorBuffer);
    float color_buffer[vertices.size()];
    for (int i = 0; i < vertices.size(); i++) {
        if (i % 3 == 0)
            color_buffer[i] = color.r;
        else if (i % 3 == 1)
            color_buffer[i] = color.g;
        else
            color_buffer[i] = color.b;
    }

    glBufferData(GL_ARRAY_BUFFER, 3 * vao_object->NumVertices * sizeof(float), color_buffer, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    ResourceManager::GetShader("object").Use();
    glm::mat4 BUTTON_MODEL = glm::mat4(1.0f);
    BUTTON_MODEL = glm::translate(BUTTON_MODEL, glm::vec3(position, 0.0f));
    BUTTON_MODEL = glm::scale(BUTTON_MODEL, glm::vec3(radius, radius, 1.0f));
    ResourceManager::GetShader("object").SetMatrix4("model", BUTTON_MODEL);

    glDrawArrays(vao_object->PrimitiveMode, 0, vao_object->NumVertices);
    glBindVertexArray(0);
}

// draw if lights are off
void Button::draw_light(int dist) {

    glBindVertexArray(vao_object->VertexArrayID);
    glBindBuffer(GL_ARRAY_BUFFER, vao_object->ColorBuffer);
    float color_buffer[vertices.size()];

    // reduce brightness according to distance from player
    float reduction_fact = 1 / (float)(pow(3, dist));
    for (int i = 0; i < vertices.size(); i++) {
        if (i % 3 == 0)
            color_buffer[i] = color.r * reduction_fact;
        else if (i % 3 == 1)
            color_buffer[i] = color.g * reduction_fact;
        else
            color_buffer[i] = color.b * reduction_fact;
    }

    glBufferData(GL_ARRAY_BUFFER, 3 * vao_object->NumVertices * sizeof(float), color_buffer, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    ResourceManager::GetShader("object").Use();
    glm::mat4 BUTTON_MODEL = glm::mat4(1.0f);
    BUTTON_MODEL = glm::translate(BUTTON_MODEL, glm::vec3(position, 0.0f));
    BUTTON_MODEL = glm::scale(BUTTON_MODEL, glm::vec3(radius, radius, 1.0f));
    ResourceManager::GetShader("object").SetMatrix4("model", BUTTON_MODEL);

    glDrawArrays(vao_object->PrimitiveMode, 0, vao_object->NumVertices);
    glBindVertexArray(0);
}