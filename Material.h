#pragma once

#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <stdlib.h>
#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"

class Texture {
public:
	unsigned int id;
	std::string type; //Difuse/Specular etc.
};

class Material
{
public:
	Material();
	~Material();
	std::string parent_mtl;
	std::string name;
	std::vector<std::string> texture_paths;
	std::vector<Texture> textures;
	glm::vec3 ambient;
	glm::vec3 specular;
	glm::vec3 diffuse;
private:

};

