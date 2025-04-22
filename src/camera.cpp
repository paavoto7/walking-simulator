#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

// constructor with vectors
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    terrainLevel = 0.0f;
    characterHeight = CHARACTERHEIGHT;
    jumpShift = JUMPHEIGHT;
    updateCameraVectors();
}

// constructor with scalar values
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    terrainLevel = 0.0f;
    characterHeight = CHARACTERHEIGHT;
    jumpShift = JUMPHEIGHT;
    updateCameraVectors();
}

// returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(Position, Position + Front, Up);
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
    glm::vec3 movementVector(0.0f);
    if (direction == FORWARD) {
        movementVector += Front;
    }
    if (direction == BACKWARD) {
        movementVector -= Front;
    }
    if (direction == LEFT) {
        movementVector -= Right;
    }
    if (direction == RIGHT) {
        movementVector += Right;
    }
    float velocity{ MovementSpeed * deltaTime };
    Position += movementVector * velocity;

    if (direction == SPACE && !isJumping) {
        jumpShift = JUMPHEIGHT;
        isJumping = true;
        Position.y += jumpShift * 1.2f * deltaTime;
    }
}

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch) {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        else if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(float yoffset) {
    Zoom -= yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    else if (Zoom > 45.0f)
        Zoom = 45.0f;
}

// Method for per-frame updations
void Camera::update(float deltatime) {
    if (isJumping) {
        updateJump(deltatime);
    }
    else if (!flyingCamera) {
        // Set the camera to a fixed height from the ground
        Position.y = terrainLevel + characterHeight;
    }
}

// calculates the front vector from the Camera's (updated) Euler Angles
void Camera::updateCameraVectors() {
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = glm::normalize(glm::cross(Right, Front));
}

// Update the jump
void Camera::updateJump(float deltatime) {
    if (Position.y <= terrainLevel + characterHeight) {
        isJumping = false;
        Position.y = terrainLevel + characterHeight;
    }
    else {
        Position.y += jumpShift * deltatime;
        jumpShift -= GRAVITY * deltatime;
    }
}
