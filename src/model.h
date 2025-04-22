#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "game_object.h"

// Class for the models loaded with Assimp
class Model: public GameObject {
public:
	float height;

	Model(const std::string& path, Shader& shader);

	void draw() override;

private:
	Shader& shader; // Main function is the owner as of now
	std::vector<Mesh> meshes;
	std::string directory;
	// Using pointers to properly share them between meshes
	std::vector<std::shared_ptr<Texture>> loadedTextures;

	float maxY = std::numeric_limits<float>::lowest();
	float minY = std::numeric_limits<float>::max();

	void loadModel(const std::string& path);

	void processNode(aiNode* node, const aiScene* scene);

	void processMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);
};


#endif // !MODEL_H
