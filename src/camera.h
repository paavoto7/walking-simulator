#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    SPACE
};

class Camera {
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // Euler Angles
    float Yaw;
    float Pitch;

    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    float characterHeight;
    float terrainLevel;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix() const;

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset);

    // Method for per-frame updations
    void update(float deltatime);

private:
    float jumpShift{ 0.4f };
    bool isJumping{ false };

    constexpr static float YAW{ -90.0f };
    constexpr static float PITCH{};
    constexpr static float SPEED{ 20.5f };
    constexpr static float SENSITIVITY{ 0.1f };
    constexpr static float ZOOM{ 45.0f };
    constexpr static bool flyingCamera{ false };
    constexpr static float CHARACTERHEIGHT{ 4.0f };
    constexpr static float GRAVITY{ 20.0f };
    constexpr static float JUMPHEIGHT{ 6.0f };

    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();

    // Update the jump
    void updateJump(float deltatime);
};

#endif // !CAMERA_H
