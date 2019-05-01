#include "Material.h"



Material::Material()
{
	//A simple class for storing material data. In future iterations of the engine, this would have functions for modifying the material data (for depths etc).
	ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	specular = glm::vec3(0.8f, 0.8f, 0.8f);
	diffuse = glm::vec3(0.8f, 0.8f, 0.8f);

	texture_paths.push_back("default_diffuse.png");
	texture_paths.push_back("default_specular.png");
	name = "DEFAULT MESH";
	parent_mtl = "DEFAULT_MTL.mtl";
}


Material::~Material()
{
}
