#pragma once
#include <sstream>
#include <iostream>
#include "Shader.h"
#include "Material.h"



struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 t_coords;
};

struct ModelData {
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> textureCoords;
	std::vector<glm::vec3> indices;
	std::vector<Material> materials;
};

class Mesh
{
public:
	Mesh();
	std::vector<Material> materials;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	void processModelData(ModelData info);
	Mesh(ModelData info);
	Texture genTexture(std::string path);
	void genNewMaterials(std::vector<Material> _materials);
	void genMatTextures();
	void bindMaterials(Shader shader);
	void draw(Shader shader, unsigned int render_mode);
	void draw(Shader shader);
	void drawInstanced(Shader shader, int count);
	~Mesh();

	std::string savedName = "";
	std::string name="";
private:
	unsigned int VAO, VBO, EBO;
	void setupMesh();
};

