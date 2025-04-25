#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>
#include <limits>
#include <memory>

#include <assimp/scene.h>
#include <glm/glm.hpp>

#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "game_object.h"

// Class for the models loaded with Assimp
class Model: public GameObject {
public:
	std::vector<Mesh> meshes;
	glm::vec3 Position{ glm::vec3(0.0f, 0.0f, 0.0f) };
	std::vector<std::shared_ptr<Texture>> loadedTextures;
	float height;

	Model(const std::string& path, Shader& shader);

	Model(const std::string& path, Shader& shader, const glm::vec3& position);

	void draw() override;

private:
	Shader& shader; // Main function is the owner as of now
	std::string directory;
	// Using pointers to properly share them between meshes

	float maxY{ std::numeric_limits<float>::lowest() };
	float minY{ std::numeric_limits<float>::max() };

	void processNode(aiNode* node, const aiScene* scene);

	void processMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);
};


#endif // !MODEL_H
