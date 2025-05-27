#include "resource_manager.h"

std::unordered_map<std::string, Shader> ResourceManager::shaderPrograms;
std::unordered_map<std::string, Model> ResourceManager::models;

Shader& ResourceManager::LoadShader(const std::string& name, const char* vertexPath, const char* fragmentPath) {
	auto shader = shaderPrograms.emplace(name, Shader(vertexPath, fragmentPath));
	// Emplace returns a pair so iterator -> to second item that is the value
	return shader.first->second;
}

Model& ResourceManager::LoadModel(const std::string& name, const std::string& path, const std::string& shaderName) {
	auto shader = models.emplace(name, Model(path, GetShader(shaderName)));
	// Emplace returns a pair so iterator -> to second item that is the value
	return shader.first->second;
}

// Returns the shader by name, if not found, throws an exception
Shader& ResourceManager::GetShader(const std::string& name) {
	return shaderPrograms.at(name);
}

Model& ResourceManager::GetModel(const std::string& name) {
	return models.at(name);
}

void ResourceManager::Clear() {
	shaderPrograms.clear();
	models.clear();
}
