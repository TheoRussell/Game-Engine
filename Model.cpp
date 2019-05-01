#include "Model.h"



//Constructors
Model::Model()
{
	name = "<DefaultName>";
	loadModel("src\\models\\block.obj", "");
}

Model::Model(std::string _name, char *path)
{
	name = _name;
	loadModel(path, "");
}

Model::Model(std::string _name, char *path, char* mat_path) {
	name = _name;
	loadModel(path, mat_path);
}

Model::Model(std::string _name, Model *model, std::string mat_path) {
	name = _name;
	meshes = model->meshes;
	meshes[0].materials.clear();
	getMaterials("src\\models\\" + mat_path, meshes[0].materials);
}

Model::Model(std::string _name, char *path, Material mats) {
	name = _name;
	std::vector<Material> material_list;
	material_list.push_back(mats);
	materials = material_list;
	loadModel(path, "");
}

Model::Model(std::string _name, char *path, std::vector<Material> mats) {
	name = _name;
	materials = mats;
	loadModel(path, "");
}

//Resetting model.
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
	//Drawing meshes with default render mode.
	if (meshes.size() > 0) {
		for (unsigned int i = 0; i < meshes.size(); i++) {
			meshes[i].draw(shader);
		}
	}

}

void Model::draw(Shader shader, unsigned int render_mode) {
	//Rendering with a render mode (ie, solid, outline).
	if (meshes.size() > 0) {
		for (unsigned int i = 0; i < meshes.size(); i++) {
			meshes[i].draw(shader, render_mode);
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
	//This is called when the user changes the mesh in the engine.
	std::vector<std::string> meshFiles;
	//The new mesh data (+ materials) must be generated.
	std::vector<Mesh> tempMeshStore = meshes;
	std::vector<Material> tempMaterialStore = materials;

	//Storing the meshes temporarily incase generation of a new mesh goes wrong.
	unsigned int index = 0;
	for (Mesh m : meshes) {
		//Adds new meshes to be loaded to a list.
		meshFiles.push_back("src\\models\\" + m.name);
		tempMeshStore[index].name = tempMeshStore[index].savedName;
		index++;
	}
	meshes.clear();

	//Loads the mesh.
	for (std::string path : meshFiles) {
		if (loadModel(path, "")) {
			materials.clear();
		}
		else {
			//Reverts the meshes back to original if error occurs.
			meshes = tempMeshStore;
			materials = tempMaterialStore;
			break;
		}
	}


}

void Model::regenerateMats(std::vector<Material> mats) {
	//Called when the user changes the materials of a mesh in the engine scene editor.
	materials = mats;
	unsigned int index = 0;
	for (Mesh m : meshes) {
		//Generates new material entities for each new material.
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
				//Vertex
				std::string newLine;
				newLine = currentLine.replace(0, 2, "") + " ";

				posStorage.push_back(getVecFromString(newLine, " "));
			}
			else if (currentLine.find("vn ") != std::string::npos) {
				//Normal
				std::string newLine;
				newLine = currentLine.replace(0, 3, "") + " ";

				normalStorage.push_back(getVecFromString(newLine, " "));
			}
			else if (currentLine.find("vt ") != std::string::npos) {
				//Texture
				std::string newLine;
				newLine = currentLine.replace(0, 3, "") + " ";

				
				glm::vec3 texCoord = getVecFromString(newLine, " ");
				texCoord.z = (float)matID;
				textureStorage.push_back(texCoord);
			}
			else if (currentLine.find("f ") != std::string::npos) {
				//Face
				std::string newLine = currentLine.replace(0, 2, "") + " ";
				std::string token;
				unsigned int pos = 0;
				pos = newLine.find(" ");
				//Iterates through each space - 3 nodes per index, seperated by spaces.
				while (pos != std::string::npos) {
					token = newLine.substr(0, pos) + "/";
					newLine.erase(0, pos + 1);
					pos = newLine.find(" ");


					glm::vec3 indice = getVecFromString(token, "/");
					indice.x -= 1; //Vertex Index
					indice.y -= 1; //Texture
					indice.z -= 1; //Normal
					if (indice.y + 1 == 0) {
						indice.y = 5000;
					}
					
					indices.push_back(indice);
				}
			}
			else if (currentLine.find("mtllib ") != std::string::npos) {
				//Declaring a material.
				std::string materialLine = currentLine.replace(0, 7, "");
				meshMaterial = materialLine;
			}
			else if (currentLine.find("usemtl ") != std::string::npos) {
				//Binding a material to the current faces.
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

		//Bundles all the model data into a single structure to pass to the mesh class.
		ModelData model;
		model.indices = indices;
		model.normals = normalStorage;
		model.positions = posStorage;
		model.textureCoords = textureStorage;
		model.materials = materials;
		//Generating a mesh from the data.
		Mesh mesh(model);
		mesh.name = path;
		mesh.savedName = path;
		meshes.push_back(mesh);
		//Close the file.
		obj.close();
		return true;
	}

	obj.close();
	return false;
}

glm::vec3 Model::getVecFromString(std::string path, std::string token) {
	unsigned int pos = 0;
	unsigned int vectorPos = 0;
	glm::vec3 vertex(0.0f, 0.0f, 0.0f); 
	std::string smallVal;
	//Finds the token.
	while ((pos = path.find(token)) != std::string::npos) {
		//Makes a subpath of the path up to the token.
		smallVal = path.substr(0, pos);
		path.erase(0, pos + 1);
		//Subpath must be new value.
		//Value is converted to a float.
		if (vectorPos == 0) {
			vertex.x = (float)atof(smallVal.c_str());
		}
		else if (vectorPos == 1) {
			vertex.y = (float)atof(smallVal.c_str());
		}
		else if (vectorPos == 2) {
			vertex.z = (float)atof(smallVal.c_str());
			return vertex;
		}
		vectorPos++;
	}

	return vertex;
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
	//Reading a .mtl file for materials.
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
				//If the previous material had no textures, add some defaults:
				textO.push_back("default_diffuse.png"); //Default texture.
				textO.push_back("default_specular.png"); //Default texture.
			}
			//Prepare variables for new material.
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
				//A period means no path was specified in the material file.
				textO.push_back("default_diffuse.png"); //Default texture.
				textO.push_back("default_specular.png"); //Default texture.
			}
			else {
				//For each _diffuse texture, the corresponding _specular texture is added to the material.
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
			//Ambient intensity;
			std::string data = f_line.replace(0, 3, "");
			mat_current.ambient = getVec3FromString(data);
		}
		else if (f_line.find("Kd ") != std::string::npos) {
			//Diffuse intensity.
			std::string data = f_line.replace(0, 3, "");
			mat_current.diffuse = getVec3FromString(data);
		}
		else if (f_line.find("Ks ") != std::string::npos) {
			//Specular intensity;
			std::string data = f_line.replace(0, 3, "");
			mat_current.specular = getVec3FromString(data);
		}
	}

	if (textO.size() == 0) {
		textO.push_back("default_diffuse.png"); //Default texture.
		textO.push_back("default_specular.png"); //Default texture.
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