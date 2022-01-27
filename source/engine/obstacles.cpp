#include "headers/obstacles.h"
#include "opengl/resource_manager.h"
using namespace std;

Obstacle::Obstacle(int n_x, int n_y, float x, float y, float width, float height, color_t color) {
    node_x = n_x;
    node_y = n_y;
    position.x = x;
    position.y = y;
    exist = false;
    this->width = width;
    this->height = height;
    this->color = color;
    // cout << this->color.r << " " << this->color.g << " " << this->color.b << endl;

    //setting up spiky vertices
    float verts[] = {
        -0.5f,
        0.5f,
        0.0f,
        0.5f,
        -0.5f,
        0.0f,
        -0.5f,
        -0.5f,
        0.0f,

        0.0f,
        0.5f,
        0.0f,
        -0.5f,
        -0.5f,
        0.0f,
        0.5f,
        -0.5f,
        0.0f,

        0.5f,
        0.5f,
        0.0f,
        -0.5f,
        -0.5f,
        0.0f,
        0.5f,
        -0.5f,
        0.0f,
    };
    for (auto v : verts)
        vertices.push_back(v);
}

//function to create vao
void Obstacle::create() {
    struct VAO *vao = new struct VAO;
    vao->PrimitiveMode = GL_TRIANGLES;
    vao->NumVertices = 9;

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

//function to draw from vao
void Obstacle::draw() {

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
    glm::mat4 OBSTACLE_MODEL = glm::mat4(1.0f);
    OBSTACLE_MODEL = glm::translate(OBSTACLE_MODEL, glm::vec3(position, 0.0f));
    OBSTACLE_MODEL = glm::scale(OBSTACLE_MODEL, glm::vec3(width, height, 1.0f));
    ResourceManager::GetShader("object").SetMatrix4("model", OBSTACLE_MODEL);

    glDrawArrays(vao_object->PrimitiveMode, 0, vao_object->NumVertices);
    glBindVertexArray(0);
}

//draw when lights are off
void Obstacle::draw_light(int dist) {

    glBindVertexArray(vao_object->VertexArrayID);
    glBindBuffer(GL_ARRAY_BUFFER, vao_object->ColorBuffer);
    float color_buffer[vertices.size()];
    float reduction_fact = 1 / (float)(pow(3, dist));
    //reduce brightness ccording to distance
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
    glm::mat4 OBSTACLE_MODEL = glm::mat4(1.0f);
    OBSTACLE_MODEL = glm::translate(OBSTACLE_MODEL, glm::vec3(position, 0.0f));
    OBSTACLE_MODEL = glm::scale(OBSTACLE_MODEL, glm::vec3(width, height, 1.0f));
    ResourceManager::GetShader("object").SetMatrix4("model", OBSTACLE_MODEL);

    glDrawArrays(vao_object->PrimitiveMode, 0, vao_object->NumVertices);
    glBindVertexArray(0);
}