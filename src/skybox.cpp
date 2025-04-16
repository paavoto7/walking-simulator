#include "skybox.h"
#include "stb_image.h"

SkyBox::SkyBox(Shader& shader, const std::string& path, const std::string& format): shader(shader) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * skyboxVertices.size(), skyboxVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    std::array<std::string, 6> sides = {
        path+"right."+format,
        path+"left."+format,
        path+"top."+format,
        path+"bottom."+format,
        path+"front."+format,
        path+"back."+format
    };

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
    
    stbi_set_flip_vertically_on_load(false);
    int width, height, nrChannels;
    for (int i = 0; i < sides.size(); ++i) {
        unsigned char* data{ stbi_load(sides[i].c_str(), &width, &height, &nrChannels, 0)};

        if (width != height) {
            std::cout << "The cubemap images need to be squares and all the same size" << std::endl;
            return;
        }

        if (data) {
            GLenum imgFormat{ GL_NONE };
            if (nrChannels == 3) {
                imgFormat = GL_RGB;
            }
            else if (nrChannels == 4) {
                imgFormat = GL_RGBA;
            }
            else {
                std::cout << "Unsupported texture format with " << nrChannels << "channels" << std::endl;
                return;
            }

            // The enum contains just ints so this works
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, imgFormat, width, height, 0, imgFormat, GL_UNSIGNED_BYTE, data
            );
        } else {
            std::cout << "Failed to load skybox side: " << sides[i] << std::endl;
            return;
        }
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

SkyBox::~SkyBox() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(1, &textureId);
}

void SkyBox::draw() {
    glDepthFunc(GL_LEQUAL);

    shader.use();
    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthFunc(GL_TRUE);
}

constexpr std::array<float, 108> SkyBox::skyboxVertices = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};