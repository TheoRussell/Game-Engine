#include "Model.h"

void Model::genID() {
	id = rand();
}


Model::Model()
{
	name = "<DefaultName>";
	loadModel("src\\models\\block.obj", "");
	//genID();
}

Model::Model(std::string _name, char *path)
{
	name = _name;
	loadModel(path, "");

	//genID();
}

Model::Model(std::string _name, char *path, char* mat_path) {
	name = _name;
	loadModel(path, mat_path);
	//genID();
}

Model::Model(std::string _name, Model *model, std::string mat_path) {
	name = _name;
	meshes = model->meshes;
	meshes[0].materials.clear();
	getMaterials("src\\models\\" + mat_path, meshes[0].materials);
	//genID();
}

Model::Model(std::string _name, char *path, Material mats) {
	name = _name;
	std::vector<Material> material_list;
	material_list.push_back(mats);
	materials = material_list;
	loadModel(path, "");
	//genID();
}

Model::Model(std::string _name, char *path, std::vector<Material> mats) {
	name = _name;
	materials = mats;
	loadModel(path, "");

	//genID();
}

void Model::reset() {
	meshes.clear();
	name = "";
	materials.clear();
}

void Model::setMesh(Mesh mesh) {
	meshes.clear();
	meshes.push_back(mesh);
}

void Model::draw(Shader shader) {
	if (meshes.size() > 0) {
		for (unsigned int i = 0; i < meshes.size(); i++) {
			meshes[i].draw(shader);
		}
	}

}

void Model::draw(Shader shader, unsigned int render_mode) {
	if (meshes.size() > 0) {
		for (unsigned int i = 0; i < meshes.size(); i++) {
			meshes[i].draw(shader, render_mode);
		}
	}

}

void Model::drawInstanced(Shader shader, int count) {
	if (meshes.size() > 0) {
		for (unsigned int i = 0; i < meshes.size(); i++) {
			meshes[i].drawInstanced(shader, count);
		}
	}

}


std::string Model::getName() {
	return name;
}
std::string Model::getPathName() {
	return pathName;
}


void Model::regenerateMeshes() {
	std::vector<std::string> meshFiles;

	std::vector<Mesh> tempMeshStore = meshes;
	std::vector<Material> tempMaterialStore = materials;

	unsigned int index = 0;
	for each (Mesh m in meshes) {
		meshFiles.push_back("src\\models\\" + m.name);
		tempMeshStore[index].name = tempMeshStore[index].savedName;
		index++;
	}
	meshes.clear();

	for each (std::string path in meshFiles) {
		if (loadModel(path, "")) {
			materials.clear();
		}
		else {
			meshes = tempMeshStore;
			materials = tempMaterialStore;
			break;
		}
	}


}

void Model::regenerateMats(std::vector<Material> mats) {
	materials = mats;
	unsigned int index = 0;
	for each (Mesh m in meshes) {
		m.genNewMaterials(mats);
		meshes[index] = m;
		index++;
	}
}


bool Model::loadModel(std::string path, std::string mat_path) {
	//Only 1 mesh!

	std::string line;
	std::vector<glm::vec3> indices;
	std::vector<glm::vec3> normalStorage;
	std::vector<glm::vec3> textureStorage;
	std::vector<glm::vec3> posStorage;

	std::fstream obj;
	obj.open(path);
	if (obj.good()) {
		std::string tempClone = path.erase(0, 11);
		tempClone = tempClone.erase(tempClone.length() - 4, 4);
		pathName = tempClone;


		std::string meshMaterial = "";
		unsigned int matID = 0;

		std::vector<std::string> matNames;


		while (std::getline(obj, line)) {
			std::string currentLine = line;

			if (currentLine.find("v ") != std::string::npos) {
				std::string newLine;
				newLine = currentLine.replace(0, 2, "") + " ";
				std::string token;
				unsigned int pos = 0;
				unsigned int vectorPos = 0;
				glm::vec3 vertex(0.0f, 0.0f, 0.0f);
				while ((pos = newLine.find(" ")) != std::string::npos) {
					token = newLine.substr(0, pos);
					newLine.erase(0, pos + 1);
					if (vectorPos == 0) {
						vertex.x = (float)atof(token.c_str());
					}
					else if (vectorPos == 1) {
						vertex.y = (float)atof(token.c_str());
					}
					else if (vectorPos == 2) {
						vertex.z = (float)atof(token.c_str());
						posStorage.push_back(vertex);
					}
					vectorPos++;
				}
			}
			else if (currentLine.find("vn ") != std::string::npos) {
				std::string newLine;
				newLine = currentLine.replace(0, 3, "") + " ";
				std::string token;
				unsigned int pos = 0;
				unsigned int vectorNorm = 0;
				glm::vec3 normalPos = glm::vec3(0.0f, 0.0f, 0.0f);
				while ((pos = newLine.find(" ")) != std::string::npos) {
					token = newLine.substr(0, pos);
					newLine.erase(0, pos + 1);
					if (vectorNorm == 0) {
						normalPos.x = (float)atof(token.c_str());
					}
					else if (vectorNorm == 1) {
						normalPos.y = (float)atof(token.c_str());
					}
					else if (vectorNorm == 2) {
						normalPos.z = (float)atof(token.c_str());
						normalStorage.push_back(normalPos);
					}
					vectorNorm++;
				}
			}
			else if (currentLine.find("vt ") != std::string::npos) {
				std::string newLine;
				newLine = currentLine.replace(0, 3, "") + " ";

				std::string token;
				unsigned int pos = 0;
				unsigned int textCoord = 0;
				glm::vec3 texCoord;
				while ((pos = newLine.find(" ")) != std::string::npos) {
					token = newLine.substr(0, pos);
					newLine.erase(0, pos + 1);
					float ref = (float)atof(token.c_str());
					if (textCoord == 0) {
						texCoord.x = ref;
					}
					else if (textCoord == 1) {
						texCoord.y = ref;
					}
					textCoord += 1;
				}
				texCoord.z = (float)matID;
				textureStorage.push_back(texCoord);
			}
			else if (currentLine.find("f ") != std::string::npos) {
				std::string newLine = currentLine.replace(0, 2, "") + " ";
				std::string token;
				unsigned int pos = 0;
				pos = newLine.find(" ");
				while (pos != std::string::npos) {
					token = newLine.substr(0, pos) + "/";
					newLine.erase(0, pos + 1);
					pos = newLine.find(" ");
					unsigned int eboPos = 0;
					unsigned int type = 0;
					std::string vertexPoint;
					glm::vec3 index = glm::vec3(0.0f, 0.0f, 0.0f);
					eboPos = token.find("/");
					while (eboPos != std::string::npos) {
						std::string token2;
						token2 = token.substr(0, eboPos);
						token.erase(0, eboPos + 1);
						eboPos = token.find("/");
						vertexPoint = token2;

						unsigned int ref = atoi(vertexPoint.c_str()) - 1;
						if (type == 0) {
							//Index
							index.x = (float)ref;
						}
						else if (type == 1) {
							//Texture
							if (ref + 1 == 0) { ref = 5000; }
							index.y = (float)ref;
						}
						else if (type == 2) {
							//Normal
							index.z = (float)ref;
							indices.push_back(index);
						}
						type++;
					}
				}
			}
			else if (currentLine.find("mtllib ") != std::string::npos) {
				std::string materialLine = currentLine.replace(0, 7, "");
				meshMaterial = materialLine;
			}
			else if (currentLine.find("usemtl ") != std::string::npos) {
				std::string materialLine = currentLine.replace(0, 7, "");
				bool previous = false;
				for each (std::string s in matNames) {
					if (s == materialLine) {
						previous = true;
					}
				}
				if (!previous) {
					matNames.push_back(materialLine);
				}
				matID++;
			}
			else {
				//Other data from the obj file.
			}
		}


		if (mat_path == "") {
			getMaterials("src\\models\\" + meshMaterial, materials);
		}
		else {
			getMaterials("src\\models\\" + mat_path, materials);
		}


		ModelData model;
		model.indices = indices;
		model.normals = normalStorage;
		model.positions = posStorage;
		model.textureCoords = textureStorage;
		model.materials = materials;
		Mesh mesh(model);
		mesh.name = path;
		mesh.savedName = path;
		meshes.push_back(mesh);

		obj.close();
		return true;
	}

	obj.close();
	return false;
	
}

glm::vec3 Model::getVec3FromString(std::string &path) {
	std::stringstream ss(path);
	glm::vec3 values = glm::vec3(0.0f,0.0f,0.0f);
	float x;
	unsigned int index = 0;
	while (ss >> x) {
		//Goes through each x,y,z
		values[index] = x;
		index++;
	}
	return values;
}

void Model::getMaterials(std::string &path, std::vector<Material> &materials) {
	std::vector<std::string> textO;
	std::vector<Material> mats;
	std::fstream mtl;
	mtl.open(path);
	std::string f_line;
	Material mat_current;
	while (std::getline(mtl, f_line)) {
		std::istringstream iss(f_line);
		if (f_line.find("newmtl ") != std::string::npos) {
			//Save previous material.
			if (textO.size() == 0) {
				textO.push_back("grass_diffuse.png"); //Default texture.
				textO.push_back("grass_specular.png"); //Default texture.
			}
			mat_current.texture_paths = textO;
			textO.clear();
			mats.push_back(mat_current);

			//New material.
			std::string mat_name = f_line.replace(0, 7, "");
			mat_current.name = mat_name;
			mat_current.parent_mtl = path;

		}
		else if (f_line.find("map_Kd ") != std::string::npos) {
			//Texture paths.
			std::string texturePath = f_line.replace(0, 7, "");
			if (texturePath == ".") {
				textO.push_back("grass_diffuse.png"); //Default texture.
				textO.push_back("grass_specular.png"); //Default texture.
			}
			else {
				textO.push_back(texturePath);
				if (f_line.find("_diffuse") != std::string::npos) {
					std::string n = texturePath.replace(texturePath.length() - 12, 12, "") + "_specular.png";
					std::ifstream hasSpecular("src\\textures\\" + n);
					if (hasSpecular.good()) {
						textO.push_back(n);
					}
					else {
						textO.push_back("default_specular.png");
					}
					hasSpecular.close();
				}
			}
		}
		else if (f_line.find("Ka ") != std::string::npos) {
			std::string data = f_line.replace(0, 3, "");
			mat_current.ambient = getVec3FromString(data);
		}
		else if (f_line.find("Kd ") != std::string::npos) {
			std::string data = f_line.replace(0, 3, "");
			mat_current.diffuse = getVec3FromString(data);
		}
		else if (f_line.find("Ks ") != std::string::npos) {
			std::string data = f_line.replace(0, 3, "");
			mat_current.specular = getVec3FromString(data);
		}
	}

	if (textO.size() == 0) {
		textO.push_back("grass_diffuse.png"); //Default texture.
		textO.push_back("grass_specular.png"); //Default texture.
	}
	mat_current.texture_paths = textO;
	mats.push_back(mat_current); //Adds the current material to the other materials in the list.

	mats.erase(mats.begin()); //Deletes empty object

	std::vector<Material> temp = materials;
	temp.insert(temp.end(), mats.begin(), mats.end());

	if (temp.size() == 0) {
		Material material;
		temp.push_back(material); //Must have a default material for textures.
	}
	//Adds any new materials to the previously stored materials.

	materials = temp;

	mtl.close();
}


Model::~Model() {

}