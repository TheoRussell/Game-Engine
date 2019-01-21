#include "Mesh.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Mesh::Mesh()
{
	
}

Mesh::Mesh(ModelData info)
{
	processModelData(info);
	setupMesh();
}



void Mesh::processModelData(ModelData info) {
	std::vector<Vertex> v;
	std::vector<unsigned int> i;
	unsigned int currentIndex = 0;
	for each ( glm::vec3 index in info.indices) {
		Vertex newV;
		newV.position = info.positions[index.x];
		if (index.y == 5000) {
			newV.t_coords = glm::vec3(0.0f, 0.0f, 0.0f);
		}
		else {
			newV.t_coords = glm::vec3(info.textureCoords[index.y]);
		}
		newV.normal = info.normals[index.z];
		v.push_back(newV);
		i.push_back(currentIndex);
		currentIndex++;
	}
	vertices = v;
	indices = i;
	materials = info.materials;

	genMatTextures();
}



void Mesh::setupMesh() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	//Vertex Pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	//Normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);
	//Textures
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, t_coords));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}


Texture Mesh::genTexture(std::string path) {
	//Creating a texture.
	unsigned int textureId;
	glGenTextures(1, &textureId); /*Want to create 1 texture and store it in the int.*/
	Texture result;
	result.id = textureId;
	if (path.find("_specular") != std::string::npos) {
		result.type = "tSpecular";
	}
	else {
		result.type = "tDiffuse";
	}

	glBindTexture(GL_TEXTURE_2D, textureId);
	stbi_set_flip_vertically_on_load(true); /*Images usually have y=0 at the top of the image, not bottom.*/
	int width, height, nrChannels;
	std::string file_path = "src\\textures\\" + path;
	const char* file_reference = file_path.c_str();
	unsigned char *data = stbi_load(file_reference, &width, &height, &nrChannels, 0);
	if (data) {
		if (nrChannels == 4) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}

		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR); /*Mipmaps are uses in down scaling to reduce memory and improve image quality.*/
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		stbi_image_free(data);
	}
	else {
		std::cout << "ERROR:TEXTURE:COULD_NOT_LOAD_FILE: " << path << std::endl;
		return genTexture("NoTexture.png");
	}


	return result;
}


void Mesh::genNewMaterials(std::vector<Material> _materials) {
	//In order to generate new texture buffers.
	materials = _materials;
	genMatTextures();

}

void Mesh::genMatTextures() {
	unsigned int index = 0;

	for each (Material m in materials) {
		
		if (m.textures.size() == 0) {
			std::vector<Texture> newTextures;
			for each (std::string s in m.texture_paths) {
				newTextures.push_back(genTexture(s));
			}
			m.textures = newTextures;
			materials[index] = m;
		}

		index++;
	}
}


void Mesh::bindMaterials(Shader shader) {
	unsigned int materialIndex = 0;
	unsigned int textureIndex = 1; //starts at 1 since 0 is for the depth map.

	genMatTextures();

	for each (Material m in materials) {
		//Loops through each texture.
		shader.setI(("material[" + std::to_string(materialIndex) + "].shininess").c_str(), 64.0f); //Gives property of texture.
		shader.setV3F(("material[" + std::to_string(materialIndex) + "].ambient").c_str(), m.ambient.x, m.ambient.y, m.ambient.z); //Gives property of texture.
		shader.setV3F(("material[" + std::to_string(materialIndex) + "].diffuse").c_str(), m.diffuse.x, m.diffuse.y, m.diffuse.z); //Gives property of texture.
		shader.setV3F(("material[" + std::to_string(materialIndex) + "].specular").c_str(), m.specular.x, m.specular.y, m.specular.z); //Gives property of texture.

		for each (Texture t in m.textures) {

			glActiveTexture(GL_TEXTURE0 + textureIndex); //Want to modify this texture.

			std::string type = t.type;
			if (type != "tDiffuse" && type != "tSpecular") {
				std::cout << "No texture type by the name of " << m.texture_paths[textureIndex -1] << std::endl;
			}
			else {
				shader.setI(("material[" + std::to_string(materialIndex) + "]." + type).c_str(), textureIndex); //Property of texture.
				glBindTexture(GL_TEXTURE_2D, t.id); //Applies texture to the current shader.
				glActiveTexture(GL_TEXTURE0 + textureIndex); //Activates texture.
				textureIndex++;
			}
		}
		materialIndex++;
	}
}


void Mesh::draw(Shader shader) {
	draw(shader, 1);
}

void Mesh::draw(Shader shader, unsigned int render_mode) {
	//RENDER_MODE ==   1 is full, 2 is wired mesh

	bindMaterials(shader);

	glBindVertexArray(VAO);
	if (render_mode == 1) {
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}
	else if (render_mode == 2) {
		glDrawElements(GL_LINE_LOOP, indices.size(), GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}

	glBindVertexArray(0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}



void Mesh::drawInstanced(Shader shader, int count) {
	unsigned int diffuseID = 0;
	unsigned int specularID = 0;
	glActiveTexture(GL_TEXTURE0);
	bindMaterials(shader);
	glBindVertexArray(VAO);
	glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, count);
	glBindVertexArray(0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}


Mesh::~Mesh() {

}