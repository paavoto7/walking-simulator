#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <unordered_map>
#include <string>

#include "shader.h"
#include "model.h"

class ResourceManager {
public:

	static Shader& LoadShader(const std::string& name, const char* vertexPath, const char* fragmentPath);
	static Model& LoadModel(const std::string& name, const std::string& path, const std::string& shaderName);

	static Shader& GetShader(const std::string& name);
	static Model& GetModel(const std::string& name);

	static void Clear();

private:
	static std::unordered_map<std::string, Shader> shaderPrograms;
	static std::unordered_map<std::string, Model> models;
};


#endif // !RESOURCEMANAGER_H
