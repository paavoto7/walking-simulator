#ifndef SKYBOX_H
#define SKYBOX_H

#include <array>

#include "game_object.h"
#include "shader.h"

class SkyBox: public GameObject {
public:
    static constexpr std::size_t vertexCount{ 36 * 3 };
    static const std::array<float, vertexCount> skyboxVertices;

    SkyBox(Shader& shader, const std::string& path = "assets/skybox/", const std::string& format = "jpg");
    ~SkyBox();

    void draw() override;
private:
    GLuint VAO, VBO;
    GLuint textureId;
    Shader& shader;
};

#endif // !SKYBOX_H
