#include "Mesh.h"

//Stb_image is an open source libary for reading .png files.
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Mesh::Mesh()
{
	
}

Mesh::Mesh(ModelData info)
{
	processModelData(info);
	setupMesh();
	savedName = name;
}



void Mesh::processModelData(ModelData info) {
	//Compiling all of the model data into vertices.
	std::vector<Vertex> v;
	std::vector<unsigned int> i;
	unsigned int currentIndex = 0;
	for ( glm::vec3 index : info.indices) {

		Vertex newV;
		//Give vertex a position from list of positions.
		newV.position = info.positions[index.x];
		if (index.y == 5000) {
			//ie, it has no texture coordinate so a basic one is added.
			newV.t_coords = glm::vec3(0.0f, 0.0f, 0.0f);
		}
		else {
			//Fetch texture coordinate from texture coord number index.y
			newV.t_coords = glm::vec3(info.textureCoords[index.y]);
		}
		//Give vertex a normal from list of normals.
		newV.normal = info.normals[index.z];
		//Push the vertex to a render list.
		v.push_back(newV);
		//Render list is referenced using the index list vv.
		i.push_back(currentIndex);
		currentIndex++;
	}
	vertices = v;
	indices = i;
	materials = info.materials;

	genMatTextures();
}



void Mesh::setupMesh() {
	//OpenGL functions to generate buffers to store all of the mesh data.
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	//Both of the buffers are static_draw entities as their data will not be frequently changing.
	//Vertex buffer object stores vertices.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	//Element buffer object stores the indices.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	//Identifying the size of each attribute in the VBO.
	//For each shader program call, the 3 attributes are sent to the shader for processing.
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
	//Identifies the type of texture.
	result.id = textureId;
	if (path.find("_specular") != std::string::npos) {
		result.type = "m_specular";
	}
	else {
		result.type = "m_diffuse";
	}

	//Binds the texture buffer.
	glBindTexture(GL_TEXTURE_2D, textureId);
	//Uses STB_Image library to load .png files.
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

	for (Material m : materials) {
		//if there are no textures in a material, only texture paths then the textures have yet to be generated.
		if (m.textures.size() == 0) {
			std::vector<Texture> newTextures;
			for (std::string s : m.texture_paths) {
				//Texture is generated from the path and added to the material.
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

	for (Material m : materials) {
		//Loops through each texture.
		//Binds the data into the shader.
		shader.setI(("material[" + std::to_string(materialIndex) + "].shininess").c_str(), 64.0f); //Gives property of texture.
		shader.setV3F(("material[" + std::to_string(materialIndex) + "].ambient").c_str(), m.ambient.x, m.ambient.y, m.ambient.z); //Gives property of texture.
		shader.setV3F(("material[" + std::to_string(materialIndex) + "].diffuse").c_str(), m.diffuse.x, m.diffuse.y, m.diffuse.z); //Gives property of texture.
		shader.setV3F(("material[" + std::to_string(materialIndex) + "].specular").c_str(), m.specular.x, m.specular.y, m.specular.z); //Gives property of texture.

		for (Texture t : m.textures) {

			glActiveTexture(GL_TEXTURE0 + textureIndex); //Want to modify this texture.

			std::string type = t.type;
			if (type != "m_diffuse" && type != "m_specular") {
				std::cout << "No texture type by the name of " << m.texture_paths[textureIndex -1] << std::endl;
			}
			else {
				shader.setI((type + "[" + std::to_string(materialIndex) + "]").c_str(), textureIndex); //Property of texture.
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

	//Binds the data buffers.
	glBindVertexArray(VAO);
	if (render_mode == 1) {
		//Renders the buffer data in the order of the indices (3 indices form 1 face triangle).
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}
	else if (render_mode == 2) {
		glDrawElements(GL_LINE_LOOP, indices.size(), GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}

	//Unbinds the data buffers.
	glBindVertexArray(0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}


Mesh::~Mesh() {

}