#include "model.h"

Model::Model(const std::string& path, Shader& shader) : shader(shader) {
	loadModel(path);
}

void Model::draw() {
	shader.use();
	for (auto& mesh : meshes) {
		mesh.draw(shader);
	}
}

// Private methods

void Model::loadModel(const std::string& path) {
	Assimp::Importer importer;
	const aiScene* scene{ importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs) };

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);

	height = maxY - minY;
}

void Model::processNode(aiNode* node, const aiScene* scene) {
	// Loop through the meshes of current node
	for (unsigned i = 0; i < node->mNumMeshes; ++i) {
		aiMesh* mesh{ scene->mMeshes[node->mMeshes[i]] };
		processMesh(mesh, scene); // Gets pushed back to meshes
	}

	// Recursively call for current nodes children
	for (unsigned j = 0; j < node->mNumChildren; ++j) {
		processNode(node->mChildren[j], scene);
	}
}

void Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<std::shared_ptr<Texture>> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
		maxY = std::max(maxY, mesh->mVertices[i].y);
		minY = std::min(minY, mesh->mVertices[i].y);
		Vertex vertex;
		vertex.Position = glm::vec3(
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z
		);
		vertex.Normal = glm::vec3(
			mesh->mNormals[i].x,
			mesh->mNormals[i].y,
			mesh->mNormals[i].z
		);

		if (mesh->mTextureCoords[0]) {
			// Using only the first texture coordinate
			vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}
		else {
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}
	// process indices
	for (unsigned j = 0; j < mesh->mNumFaces; ++j) {
		const aiFace& face{ mesh->mFaces[j] };
		for (unsigned k = 0; k < face.mNumIndices; ++k) {
			indices.push_back(face.mIndices[k]);
		}
	}
	// process material
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* mat{ scene->mMaterials[mesh->mMaterialIndex] };
		std::vector<std::shared_ptr<Texture>> diffuseMaps = loadMaterialTextures(mat, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<std::shared_ptr<Texture>> specularMaps = loadMaterialTextures(mat, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	meshes.push_back(Mesh(vertices, indices, textures));
}

std::vector<std::shared_ptr<Texture>> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName) {
	std::vector<std::shared_ptr<Texture>> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);

		// Do the following to avoid creating Texture objects for same textures
		bool alreadyLoaded = false;
		for (unsigned int j = 0; j < loadedTextures.size(); ++j) {
			if (std::strcmp(loadedTextures[j]->path.c_str(), str.C_Str()) == 0) {
				textures.push_back(loadedTextures[j]);
				alreadyLoaded = true;
				break;
			}
		}

		if (!alreadyLoaded) {
			std::shared_ptr<Texture> texture{ std::make_shared<Texture>(str.C_Str(), directory, false) };
			texture->type = typeName;
			texture->path = str.C_Str();
			textures.push_back(texture);
			loadedTextures.push_back(texture);
		}

	}
	return textures;
}
