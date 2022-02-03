#ifndef CAMERA_H
#define CAMERA_H

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <bits/stdc++.h>

//different camera movements
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

enum Spin_Movement {
    RIGHT_SPIN,
    LEFT_SPIN
};

enum Views {
    FRONT_VIEW,
    SIDE_VIEW,
    TOP_VIEW
};

// Default camera values
const float SPEED = 1.5f;
const glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);

// Camera class
class Camera {
public:
    // camera Attributes
    glm::vec3 Position; //camera position according to world
    glm::vec3 Front;    // vector where camera is looking at
    glm::vec3 Up;       // up vector of camera
    glm::vec3 Right;    // right vector of camera (front x WORLD_UP)

    // camera options
    float MovementSpeed;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f)) : MovementSpeed(SPEED) {
        Position = position;
        updateCameraVectors(front);
    }
    // returns the view matrix calculated using the LookAt Matrix
    glm::mat4 GetViewMatrix() {
        return glm::lookAt(Position, Position + Front, WORLD_UP);
    }

    // walk around camera
    void walk(Camera_Movement direction, float deltaTime) {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
        if (direction == UP)
            Position += Up * velocity;
        if (direction == DOWN)
            Position -= Up * velocity;
    }

    //points at object
    void pointToObject(glm::vec3 objectOrigin) {
        // the new front vector is obtained by normalizing the difference between object origin and camera position
        glm::vec3 newFront = glm::normalize(objectOrigin - Position);
        updateCameraVectors(newFront);
    }

    //spins camera
    void spin(Spin_Movement spin_direction, float deltaTime, glm::vec3 objectOrigin) {
        pointToObject(objectOrigin);
        float velocity = 1.4 * MovementSpeed * deltaTime;
        if (spin_direction == RIGHT_SPIN)
            Position += Right * velocity;
        if (spin_direction == LEFT_SPIN)
            Position -= Right * velocity;
    }

    //predefined views
    void setView(Views view, float deltaTime, glm::vec3 objectOrigin) {
        glm::vec3 newPosition;
        if (view == FRONT_VIEW) {
            newPosition = glm::vec3(objectOrigin.x, objectOrigin.y, objectOrigin.z + 5.0f);
        }
        if (view == SIDE_VIEW) {
            newPosition = glm::vec3(objectOrigin.x + 5.0f, objectOrigin.y, objectOrigin.z);
        }
        if (view == TOP_VIEW) {
            newPosition = glm::vec3(objectOrigin.x, objectOrigin.y + 5.0f, objectOrigin.z + 0.05f);
        }
        Position = newPosition;
        pointToObject(objectOrigin);
    }

private:
    // calculates the front vector and other camera coordinate vectors
    void updateCameraVectors(glm::vec3 front) {
        // calculate the new Front vector
        Front = front;
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WORLD_UP));
        Up = glm::normalize(glm::cross(Right, Front));
    }
};
#endif