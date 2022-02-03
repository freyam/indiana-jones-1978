#include "headers/imposter.h"
#include "opengl/resource_manager.h"
using namespace std;

Imposter::Imposter(float x, float y, float width, float height, color_t color, float speed) {
    position.x = x;
    position.y = y;
    this->width = width;
    this->height = height;
    this->color = color;
    this->exist = true;
    this->speed = speed;
    // cout << this->color.r << " " << this->color.g << " " << this->color.b << endl;

    // setting up rectangel and helmet vertices
    float verts[] = {
        // first triangle
        -0.5f, 0.5f, 0.0f,  // top left
        0.5f, 0.5f, 0.0f,   //top right
        -0.5f, -0.5f, 0.0f, //bottom left

        //second triangle
        -0.5f, -0.5f, 0.0f, // bbottom left
        0.5f, -0.5f, 0.0f,  // bottom right
        0.5f, 0.5f, 0.0f    // top right
    };
    for (auto v : verts)
        vertices.push_back(v);

    int semVerts = 182;
    float dPi = M_PI * 2.0f;

    float semicircleX[semVerts];
    float semicircleY[semVerts];
    float semicircleZ[semVerts];

    semicircleX[0] = 0.0f;
    semicircleY[0] = 0.1f;
    semicircleZ[0] = 0.0f;

    for (int i = 1; i < semVerts; i++) {
        semicircleX[i] = 0.3 * cos(i * dPi / 360);
        semicircleY[i] = 0.1 + (0.3 * sin(i * dPi / 360));
        semicircleZ[i] = 0.0f;
    }

    float total_semverts[3 * semVerts];

    for (int i = 0; i < semVerts; i++) {
        total_semverts[i * 3] = semicircleX[i];
        total_semverts[i * 3 + 1] = semicircleY[i];
        total_semverts[i * 3 + 2] = semicircleZ[i];
    }

    for (auto v : total_semverts)
        vertices.push_back(v);
}

// function to create vao
void Imposter::create() {
    struct VAO *vao = new struct VAO;
    vao->PrimitiveMode = GL_TRIANGLES;
    vao->NumVertices = 6;

    glGenVertexArrays(1, &(vao->VertexArrayID));

    glGenBuffers(1, &(vao->VertexBuffer));
    glGenBuffers(1, &(vao->ColorBuffer));

    glBindVertexArray(vao->VertexArrayID);

    glBindBuffer(GL_ARRAY_BUFFER, vao->VertexBuffer);
    float vertex_buffer[3 * vao->NumVertices];
    for (int i = 0; i < (3 * vao->NumVertices); i++)
        vertex_buffer[i] = vertices[i];

    glBufferData(GL_ARRAY_BUFFER, 3 * vao->NumVertices * sizeof(float), vertex_buffer, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    vao_object = vao;
}

// function to draw from vao
void Imposter::draw() {
    glBindVertexArray(vao_object->VertexArrayID);
    glBindBuffer(GL_ARRAY_BUFFER, vao_object->ColorBuffer);
    float color_buffer[3 * vao_object->NumVertices];
    for (int i = 0; i < (3 * vao_object->NumVertices); i++) {
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
    glm::mat4 PLAYER_MODEL = glm::mat4(1.0f);
    PLAYER_MODEL = glm::translate(PLAYER_MODEL, glm::vec3(position, 0.0f));
    PLAYER_MODEL = glm::scale(PLAYER_MODEL, glm::vec3(width, height, 1.0f));
    ResourceManager::GetShader("object").SetMatrix4("model", PLAYER_MODEL);

    glDrawArrays(vao_object->PrimitiveMode, 0, 6);
    glBindVertexArray(0);

    //helmet
    struct VAO *vao_helm = new struct VAO;
    vao_helm->PrimitiveMode = GL_TRIANGLE_FAN;
    vao_helm->NumVertices = 182;

    glGenVertexArrays(1, &(vao_helm->VertexArrayID));

    glGenBuffers(1, &(vao_helm->VertexBuffer));
    glGenBuffers(1, &(vao_helm->ColorBuffer));

    glBindVertexArray(vao_helm->VertexArrayID);

    glBindBuffer(GL_ARRAY_BUFFER, vao_helm->VertexBuffer);
    float vertex_buffer_helm[3 * vao_helm->NumVertices];
    for (int i = 18; i < vertices.size(); i++)
        vertex_buffer_helm[i - 18] = vertices[i];

    glBufferData(GL_ARRAY_BUFFER, 3 * vao_helm->NumVertices * sizeof(float), vertex_buffer_helm, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vao_helm->ColorBuffer);
    float color_buffer_helm[3 * vao_helm->NumVertices];
    color_buffer_helm[0] = COLOR_GREY.r;
    color_buffer_helm[1] = COLOR_GREY.g;
    color_buffer_helm[2] = COLOR_GREY.b;
    for (int i = 3; i < (3 * vao_helm->NumVertices); i++) {
        if (i % 3 == 0)
            color_buffer_helm[i] = COLOR_BLACK.r;
        else if (i % 3 == 1)
            color_buffer_helm[i] = COLOR_BLACK.g;
        else
            color_buffer_helm[i] = COLOR_BLACK.b;
    }
    glBufferData(GL_ARRAY_BUFFER, 3 * vao_helm->NumVertices * sizeof(float), color_buffer_helm, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDrawArrays(vao_helm->PrimitiveMode, 0, vao_helm->NumVertices);
    glBindVertexArray(0);
}

// draw when lights are off
void Imposter::draw_light(int dist) {
    glBindVertexArray(vao_object->VertexArrayID);
    glBindBuffer(GL_ARRAY_BUFFER, vao_object->ColorBuffer);
    float color_buffer[3 * vao_object->NumVertices];
    float reduction_factor = 1 / (float)(pow(3, dist));
    // reduce brightness according to distance
    for (int i = 0; i < (3 * vao_object->NumVertices); i++) {
        if (i % 3 == 0)
            color_buffer[i] = color.r * reduction_factor;
        else if (i % 3 == 1)
            color_buffer[i] = color.g * reduction_factor;
        else
            color_buffer[i] = color.b * reduction_factor;
    }
    glBufferData(GL_ARRAY_BUFFER, 3 * vao_object->NumVertices * sizeof(float), color_buffer, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    ResourceManager::GetShader("object").Use();
    glm::mat4 PLAYER_MODEL = glm::mat4(1.0f);
    PLAYER_MODEL = glm::translate(PLAYER_MODEL, glm::vec3(position, 0.0f));
    PLAYER_MODEL = glm::scale(PLAYER_MODEL, glm::vec3(width, height, 1.0f));
    ResourceManager::GetShader("object").SetMatrix4("model", PLAYER_MODEL);

    glDrawArrays(vao_object->PrimitiveMode, 0, 6);
    glBindVertexArray(0);

    //helmet
    struct VAO *vao_helm = new struct VAO;
    vao_helm->PrimitiveMode = GL_TRIANGLE_FAN;
    vao_helm->NumVertices = 182;

    glGenVertexArrays(1, &(vao_helm->VertexArrayID));

    glGenBuffers(1, &(vao_helm->VertexBuffer));
    glGenBuffers(1, &(vao_helm->ColorBuffer));

    glBindVertexArray(vao_helm->VertexArrayID);

    glBindBuffer(GL_ARRAY_BUFFER, vao_helm->VertexBuffer);
    float vertex_buffer_helm[3 * vao_helm->NumVertices];
    for (int i = 18; i < vertices.size(); i++)
        vertex_buffer_helm[i - 18] = vertices[i];

    glBufferData(GL_ARRAY_BUFFER, 3 * vao_helm->NumVertices * sizeof(float), vertex_buffer_helm, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vao_helm->ColorBuffer);
    float color_buffer_helm[3 * vao_helm->NumVertices];
    color_buffer_helm[0] = COLOR_GREY.r * reduction_factor;
    color_buffer_helm[1] = COLOR_GREY.g * reduction_factor;
    color_buffer_helm[2] = COLOR_GREY.b * reduction_factor;
    for (int i = 3; i < (3 * vao_helm->NumVertices); i++) {
        if (i % 3 == 0)
            color_buffer_helm[i] = COLOR_BLACK.r;
        else if (i % 3 == 1)
            color_buffer_helm[i] = COLOR_BLACK.g;
        else
            color_buffer_helm[i] = COLOR_BLACK.b;
    }
    glBufferData(GL_ARRAY_BUFFER, 3 * vao_helm->NumVertices * sizeof(float), color_buffer_helm, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDrawArrays(vao_helm->PrimitiveMode, 0, vao_helm->NumVertices);
    glBindVertexArray(0);
}