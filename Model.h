#pragma once
#include "Mesh.h"
#include "Material.h"

#include <cstdlib> //Random numbers.

class Model
{
public:
	Model(std::string name, char *path);
	Model(std::string name, char * path, char * mat_path);
	Model(std::string name, Model * model, std::string mat_path);
	Model(std::string name, char * path, Material mats);
	Model(std::string name, char * path, std::vector<Material> mats);
	void reset();
	void setMesh(Mesh mesh);
	Model();
	void draw(Shader shader);
	void draw(Shader shader, unsigned int render_mode);

	void getMaterials(std::string &path, std::vector<Material> &materials);


	bool loadModel(std::string path, std::string mat_path);
	glm::vec3 getVecFromString(std::string path, std::string token);
	~Model();
	std::string getName();
	std::string getPathName();

	void regenerateMeshes();
	void regenerateMats(std::vector<Material> mats);

	int id;

	std::vector<Material> materials;
	std::vector<Mesh> meshes;
private:

	std::string name;
	std::string pathName;
	
	glm::vec3 getVec3FromString(std::string & path);
};

