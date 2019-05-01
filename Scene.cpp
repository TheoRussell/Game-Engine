#include "Scene.h"

#include <GLFW/glfw3.h>

#define SCENE_H

Scene::Scene()
{
	defaultCamera();
	name = "Scene";
}

Scene::~Scene()
{
}

void Scene::defaultCamera() {
	//Adds a default camera object to the scene to move around.
	unsigned int playerID = addObject(Object());
	setObjectPos(playerID, glm::vec3(0.0f, 1.0f, 0.0f));
	objects[playerID].components.addCamera(Camera());
	objects[playerID].components.cameras[0].componentTransform.name = "Camera";
	objects[playerID].physicsBody.doesGravity = false;
	objects[playerID].select();
}

void Scene::deleteObjects() {
	objects.clear();
}

void Scene::disableCameras() {
	//Iterates through each object and disables the camera components.
	if (objects.size() > 0) {
		for (unsigned int objID = 0; objID < objects.size(); objID++) {
			if (objects[objID].components.cameras.size() > 0) {
				for (unsigned int camID = 0; camID < objects[objID].components.cameras.size(); camID++) {
					objects[objID].components.cameras[camID].camera.enabled = false;
				}
			}
		}
	}

}

CamComponent Scene::getCamera() {
	return getCamera(false);
}



CamComponent Scene::getCamera(bool inGame) {
	//Camera caching is important as it means that the user's game camera and inspector camera are seperate entities.
	//They can easily be switched out when the user changes between engine perspectives.

	//If in game, returns the cached game cam (ie, the camera in the scene in an object).
	if (inGame && cachedGame) {
		return cachedGameCam;
	}
	else if (inGame && !cachedGame) {
		//If there is no cached camera, find the current active camera in the scene and set it to be that.
		for (unsigned int objID = 0; objID < objects.size(); objID++) {
			for (unsigned int camID = 0; camID < objects[objID].components.cameras.size(); camID++) {
				CamComponent cam = objects[objID].components.cameras[camID];
				cam.componentTransform.objID = objID;
				if (cam.camera.enabled) {
					cachedGame = true;
					cachedGameCam = cam;
					return cachedGameCam;
				}
			}
		}
	} else if (!inGame) {
		//Returns the game preview camera. (that belongs to no object).
		return cachedEditorCam;
	}
	defaultCamera();
	return getCamera();
}

void Scene::deleteScene(std::string path) {
	try {
		std::remove(path.c_str());
		scene_deleted = true;
	}
	catch (std::exception ex) {
		std::cout << "Could not delete world, " << ex.what() << std::endl;
	}
}

bool Scene::loadBinary(std::string WorkingDir) {
	//Loads the scene from a binary file. Utilises the Binary Files class functions.
	try {
		objects.clear();
		cachedGame = false;
		//Ensures the file path is valid.
		if (WorkingDir.find(".SCENE") == std::string::npos) {
			WorkingDir.append(".SCENE");
		}

		std::ifstream file(WorkingDir);
		if (file.good()) {
			//Reads the scene data.
			name = BinaryFiles::getString(file);
			voidCol = BinaryFiles::getVec3(file);
			d_ambient = BinaryFiles::getVec3(file);
			d_diffuse = BinaryFiles::getVec3(file);
			d_specular = BinaryFiles::getVec3(file);
			//Iterates through each block to the end of the file, finding all possible objects.
			BINARYWrite bw = BINARY_Label;
			while (bw != BINARY_END) {
				bw = BinaryFiles::getBINARYType(file);
				if (bw == BINARY_Object) {
					objects.push_back(loadObject(file, false));
				}
			}
			file.close();
		}
		return true;
	}
	catch (std::exception ex) {
		//This will occur if the file path is incorrect, or there are unexpected values in the file stream that alter what data types the loader thinks it is reading.
		std::cout << "The save file is corrupt! Sorry: " << ex.what() << std::endl;
		return false;
	}

	return false;
}

Object Scene::loadObject(std::ifstream &file, bool readType) {
	BINARYWrite bw;
	if (readType) {
		bw = BinaryFiles::getBINARYType(file);
	}
	else {
		bw = BINARY_Object;
	}
	
	Object obj;
	//Loads scene transformation.
	obj.name = BinaryFiles::getString(file);
	obj.pos = BinaryFiles::getVec3(file);
	obj.sca = BinaryFiles::getVec3(file);
	obj.roll = BinaryFiles::getFloat(file);
	obj.pitch = BinaryFiles::getFloat(file);
	obj.yaw = BinaryFiles::getFloat(file);
	//Physics data.
	obj.physicsBody.enabled = BinaryFiles::getBool(file);
	obj.physicsBody.collides = BinaryFiles::getBool(file);
	obj.physicsBody.doesGravity = BinaryFiles::getBool(file);

	obj.physicsBody.mass = BinaryFiles::getFloat(file);

	obj.physicsBody.position = BinaryFiles::getVec3(file);
	obj.physicsBody.scale = BinaryFiles::getVec3(file);
	obj.physicsBody.roll = BinaryFiles::getFloat(file);
	obj.physicsBody.pitch = BinaryFiles::getFloat(file);
	obj.physicsBody.yaw = BinaryFiles::getFloat(file);

	while (bw != BINARY_EOO) { //Continues through the data till the end of the buffer is found or End Of Object.
		bw = BinaryFiles::getBINARYType(file);
		if (bw < BINARY_Physics) {
			//Error handling - this shouldn't occur. < Physics is any of the data structures before an object's components.
			file.close();
			throw std::exception("File does not follow file protocols");
		}

		if (bw == BINARY_Physics) {
			//Collision box.
			BoxCollider bc;
			bc.position = BinaryFiles::getVec3(file);
			bc.scale = BinaryFiles::getVec3(file);
			bc.roll = BinaryFiles::getFloat(file);
			bc.pitch = BinaryFiles::getFloat(file);
			bc.yaw = BinaryFiles::getFloat(file);

			int cornersCount = BinaryFiles::getInt(file);
			if (cornersCount > 0) {
				for (int i = 0; i < cornersCount; i++) {
					bc.corners.push_back(BinaryFiles::getVec3(file));
				}
			}
			obj.physicsBody.coll_Box.push_back(bc);
		}
		if (bw == BINARY_Camera) {
			//Camera components.
			CamComponent cc;
			cc.componentTransform.name = BinaryFiles::getString(file);
			cc.componentTransform.position = BinaryFiles::getVec3(file);
			cc.componentTransform.roll = BinaryFiles::getFloat(file);
			cc.componentTransform.pitch = BinaryFiles::getFloat(file);
			cc.componentTransform.yaw = BinaryFiles::getFloat(file);
			cc.camera.setLookSensitivity(BinaryFiles::getFloat(file));
			cc.camera.setMoveSensitivity(BinaryFiles::getFloat(file));
			cc.camera.locked = BinaryFiles::getBool(file);
			obj.components.cameras.push_back(cc);
		}
		if (bw == BINARY_PLight) {
			//Point lights.
			PLightComponent pl;
			pl.componentTransform.name = BinaryFiles::getString(file);
			pl.componentTransform.position = BinaryFiles::getVec3(file);
			pl.light.colour_amb = BinaryFiles::getVec3(file);
			pl.light.colour_diff = BinaryFiles::getVec3(file);
			pl.light.colour_spec = BinaryFiles::getVec3(file);
			pl.light.linear = BinaryFiles::getFloat(file);
			pl.light.quadratic = BinaryFiles::getFloat(file);
			pl.light.active = BinaryFiles::getBool(file);
			obj.components.pointLights.push_back(pl);
		}
		if (bw == BINARY_Model) {
			ModelComponent mc;
			mc.componentTransform.name = BinaryFiles::getString(file);
			mc.componentTransform.position = BinaryFiles::getVec3(file);
			mc.componentTransform.scale = BinaryFiles::getVec3(file);
			mc.componentTransform.roll = BinaryFiles::getFloat(file);
			mc.componentTransform.pitch = BinaryFiles::getFloat(file);
			mc.componentTransform.yaw = BinaryFiles::getFloat(file);

			mc.modelData.meshes.clear();
			//Adding meshes.
			int meshCount = BinaryFiles::getInt(file);
			if (meshCount > 0) {
				for (int meshI = 0; meshI < meshCount; meshI++) {
					Mesh mesh;
					mesh.name = BinaryFiles::getString(file);
					mc.modelData.meshes.push_back(mesh);
				}
			}
			//Adding material to mesh
			std::vector<Material> mats;
			int matCount = BinaryFiles::getInt(file);
			if (matCount > 0) {
				for (int matI = 0; matI < matCount; matI++) {
					Material material;
					material.ambient = BinaryFiles::getVec3(file);
					material.diffuse = BinaryFiles::getVec3(file);
					material.specular = BinaryFiles::getVec3(file);
					material.name = BinaryFiles::getString(file);

					material.texture_paths.clear();
					//Adding textures to material.
					int textCount = BinaryFiles::getInt(file);
					if (textCount > 0) {
						for (int i = 0; i < textCount; i++) {
							std::string texturePath = BinaryFiles::getString(file);
							material.texture_paths.push_back(texturePath);
						}
					}
					mats.push_back(material);
				}
			}
			//When the mesh is complete, it calls upon the model class to generate the mesh and material data buffers.
			mc.modelData.regenerateMeshes();
			mc.modelData.regenerateMats(mats);

			obj.components.models.push_back(mc);
		}
		if (bw == BINARY_Script) {
			obj.addScript(BinaryFiles::getString(file));
		}

	}

	return obj;
}

bool Scene::saveBinary(std::string WorkingDir) {

	int zero = 0;
	try {
		std::ofstream file(WorkingDir + "Scenes\\" + name + ".SCENE");
		if (file.good()) {

			BinaryFiles::writeString(file, name);
			BinaryFiles::writeVec3(file, voidCol);
			//Writing directional light.
			BinaryFiles::writeVec3(file, d_ambient);
			BinaryFiles::writeVec3(file, d_diffuse);
			BinaryFiles::writeVec3(file, d_specular);

			//Objects
			BINARYWrite bw = BINARY_Object;
			for (Object obj : objects) {
				saveObject(obj, file);
			}

			//End of file.
			bw = BINARY_END;
			BinaryFiles::writeBINARYType(file, bw);
		}

		file.close();
		return true;
	}
	catch (std::exception ex) {
		std::cout << ex.what() << std::endl;
		return false;
	}
}

void Scene::saveObject(Object obj, std::ofstream &file) {
	BINARYWrite bw = BINARY_Object;
	BinaryFiles::writeBINARYType(file, bw);
	BinaryFiles::writeString(file, obj.name);
	BinaryFiles::writeVec3(file, obj.pos);
	BinaryFiles::writeVec3(file, obj.sca);
	BinaryFiles::writeFloat(file, obj.roll);
	BinaryFiles::writeFloat(file, obj.pitch);
	BinaryFiles::writeFloat(file, obj.yaw);

	//Physics of object.
	BinaryFiles::writeBool(file, obj.physicsBody.enabled);
	BinaryFiles::writeBool(file, obj.physicsBody.collides);
	BinaryFiles::writeBool(file, obj.physicsBody.doesGravity);
	BinaryFiles::writeFloat(file, obj.physicsBody.mass);
	BinaryFiles::writeVec3(file, obj.physicsBody.position);
	BinaryFiles::writeVec3(file, obj.physicsBody.scale);
	BinaryFiles::writeFloat(file, obj.physicsBody.roll);
	BinaryFiles::writeFloat(file, obj.physicsBody.pitch);
	BinaryFiles::writeFloat(file, obj.physicsBody.yaw);
	for each (BoxCollider bc in obj.physicsBody.coll_Box) {
		bw = BINARY_Physics;
		BinaryFiles::writeBINARYType(file, bw);
		BinaryFiles::writeVec3(file, bc.position);
		BinaryFiles::writeVec3(file, bc.scale);
		BinaryFiles::writeFloat(file, bc.roll);
		BinaryFiles::writeFloat(file, bc.pitch);
		BinaryFiles::writeFloat(file, bc.yaw);

		int cornerQuant = bc.corners.size();
		BinaryFiles::writeInt(file, cornerQuant);
		for each (glm::vec3 corner in bc.corners) {
			BinaryFiles::writeVec3(file, corner);
		}
	}
	//Components
	for each (CamComponent cc in obj.components.cameras) {
		bw = BINARY_Camera;
		BinaryFiles::writeBINARYType(file, bw);
		BinaryFiles::writeString(file, cc.componentTransform.name);
		BinaryFiles::writeVec3(file, cc.componentTransform.position);
		BinaryFiles::writeFloat(file, cc.componentTransform.roll);
		BinaryFiles::writeFloat(file, cc.componentTransform.pitch);
		BinaryFiles::writeFloat(file, cc.componentTransform.yaw);
		float lookSpeed = cc.camera.getLookSensitivity();
		float moveSpeed = cc.camera.getMoveSensitivity();
		BinaryFiles::writeFloat(file, lookSpeed);
		BinaryFiles::writeFloat(file, moveSpeed);
		BinaryFiles::writeBool(file, cc.camera.locked);

	}
	for each (PLightComponent pl in obj.components.pointLights) {
		bw = BINARY_PLight;
		BinaryFiles::writeBINARYType(file, bw);
		BinaryFiles::writeString(file, pl.componentTransform.name);
		BinaryFiles::writeVec3(file, pl.componentTransform.position);
		BinaryFiles::writeVec3(file, pl.light.colour_amb);
		BinaryFiles::writeVec3(file, pl.light.colour_diff);
		BinaryFiles::writeVec3(file, pl.light.colour_spec);
		BinaryFiles::writeFloat(file, pl.light.linear);
		BinaryFiles::writeFloat(file, pl.light.quadratic);
		BinaryFiles::writeBool(file, pl.light.active);
	}
	for each (ModelComponent mc in obj.components.models) {
		bw = BINARY_Model;
		BinaryFiles::writeBINARYType(file, bw);
		BinaryFiles::writeString(file, mc.componentTransform.name);
		BinaryFiles::writeVec3(file, mc.componentTransform.position);
		BinaryFiles::writeVec3(file, mc.componentTransform.scale);
		BinaryFiles::writeFloat(file, mc.componentTransform.roll);
		BinaryFiles::writeFloat(file, mc.componentTransform.pitch);
		BinaryFiles::writeFloat(file, mc.componentTransform.yaw);
		//Meshes
		int meshCount = mc.modelData.meshes.size();
		BinaryFiles::writeInt(file, meshCount);
		for each (Mesh mesh in mc.modelData.meshes) {
			BinaryFiles::writeString(file, mesh.name);
		}

		//Materials
		int matCount = mc.modelData.materials.size();
		BinaryFiles::writeInt(file, matCount);
		for each (Material mat in mc.modelData.materials) {
			BinaryFiles::writeVec3(file, mat.ambient);
			BinaryFiles::writeVec3(file, mat.diffuse);
			BinaryFiles::writeVec3(file, mat.specular);
			BinaryFiles::writeString(file, mat.name);
			//Textures
			int textureCount = mat.texture_paths.size();
			BinaryFiles::writeInt(file, textureCount);
			for each (std::string tPath in mat.texture_paths) {
				BinaryFiles::writeString(file, tPath);
			}
		}
	}
	for (std::string scriptID : obj.componentScriptIDs) {
		bw = BINARY_Script;
		BinaryFiles::writeBINARYType(file, bw);
		BinaryFiles::writeString(file, scriptID);
	}

	bw = BINARY_EOO;
	BinaryFiles::writeBINARYType(file, bw);
}


int Scene::getSelected() {
	unsigned int index = 0;
	for each (Object obj in objects) {
		if (obj.isSelected()) {
			return (int)index;
		}
		index++;
	}
	return -1;
}

void Scene::deselectAll() {
	for each (Object obj in objects) {
		obj.deselect();
	}
}

unsigned int Scene::getObjectsCount() {
	return objects.size();
}

Object& Scene::getObject(unsigned int id) {
	if (objects.size() > 0) {
		return objects[id];
	}
}

bool Scene::deleteObject(unsigned int id) {
	try {
		if (objects.size() > 0) {
			if (id < objects.size()) {
				for (unsigned int camID = 0; camID < objects[id].components.cameras.size(); camID++) {
					CamComponent cam = objects[id].components.cameras[camID];
					if (cam.camera.enabled && cachedGame && id == cachedGameCam.componentTransform.objID) {
						cachedGame = false;
					}
				}
				objects.erase(objects.begin() + id);
				return true;
			}
		}
		
		return false;
	}
	catch (std::exception ex) {
		std::cout << "Failed to delete object from scene" << id << std::endl;
		return false;
	}	
}

std::vector<PLightComponent> Scene::getPointLights() {
	std::vector<PLightComponent> plights;
	for each (Object obj in objects) {
		for each (PLightComponent light in obj.components.getPointLights()) 
		{
			if (plights.size() > MAX_POINTLIGHTS) {
				std::cout << "Ran out of point lights (max = " << MAX_POINTLIGHTS << ")" << std::endl;
			}
			else {
				plights.push_back(light);
			}

		}
	}
	return plights;
}




void Scene::select(unsigned int ID) {
	unsigned int index = 0;
	for each (Object obj in objects) {
		if (index == ID) {
			objects[ID].select();
		}
		else {
			objects[index].deselect();
		}
		index++;
	}
}

void Scene::deselect(unsigned int ID) {
	if (ID < objects.size()) {
		objects[ID].deselect();
	}

}

bool Scene::isSelected(unsigned int ID) {
	if (ID < objects.size()) {
		return objects[ID].isSelected();
	}
	return false;
}

std::vector<Object> Scene::getObjects() {
	return objects;
}


unsigned int Scene::addObject(Object o) {
	if (objects.size() == 0) {
		return addObject(o, "obj0", false);
	}
	return addObject(o, "obj" + std::to_string(objects.size() - 1), false);
}

unsigned int Scene::addObject(Object o, bool isActor) {
	return addObject(o, "obj" + std::to_string(objects.size() - 1), isActor);
}

unsigned int Scene::addObject(Object o, std::string name) {
	return addObject(o, name, false);
}


Scene Scene::interpolate(Scene other, double deltaTime) {
	//Interpolating between one scene and the next.
		Scene s;
		s.cachedEditorCam = (cachedEditorCam - other.cachedEditorCam) * deltaTime;
		s.cachedGameCam = (cachedGameCam - other.cachedGameCam) * deltaTime;
		s.d_ambient = d_ambient;
		s.d_diffuse = d_diffuse;
		s.d_specular = d_specular;

		std::vector<Object> newObjects;
		for each (Object obj in objects) {
			bool foundMatch = false;
			for (Object other_obj : other.objects) {
				if (obj.name == other_obj.name) {
					Object o = obj - other_obj;
					o = o * deltaTime;
					newObjects.push_back(o);
					foundMatch = true;
					break;
				}
			}
			if (!foundMatch) {
				//If the objects are not the same, it adds the new object to the scene.
				newObjects.push_back(obj);
			}
		}
		s.objects = objects;
		s.voidCol = voidCol;
		return s;
	
}

unsigned int Scene::addObject(Object o, std::string name, bool isActor) {
	o.name = name;
	deselectAll();
	o.select();

	if (isActor) {
		//Actors do not have an obj ID that they belong to. Hence, each component needs to be updated.
		for (int i = 0; i < o.components.models.size(); i++) {
			o.components.models[i].componentTransform.objID = objects.size() - 1;
		}
		for (int i = 0; i < o.components.pointLights.size(); i++) {
			o.components.pointLights[i].componentTransform.objID = objects.size() - 1;
		}
		for (int i = 0; i < o.components.cameras.size(); i++) {
			o.components.cameras[i].componentTransform.objID = objects.size() - 1;
		}
	}
	objects.push_back(o);
	return objects.size() - 1;
}

void Scene::setObject(unsigned int ID, Object & object)
{
	if (ID < objects.size()) {
		objects[ID] = object;
	}

}

void Scene::setObjectScale(unsigned int ID, glm::vec3 scale) {
	if (ID < objects.size()) {
		objects[ID].setScale(scale);
	}

}

void Scene::setObjectPos(unsigned int ID, glm::vec3 pos) {
	if (ID < objects.size()) {
		objects[ID].setPos(pos);
	}

}

void Scene::setObjectRot(unsigned int ID, std::string axis, float angle) {
	if (ID < objects.size()) {
		objects[ID].setRot(axis, angle);
	}

}

void Scene::scaleObject(unsigned int ID, glm::vec3 scale) {
	if (ID < objects.size()) {
		objects[ID].scale(scale);
	}

}

void Scene::moveObject(unsigned int ID, glm::vec3 pos) {
	if (ID < objects.size()) {
		objects[ID].move(pos);
	}

}

void Scene::rotateObject(unsigned int ID, std::string axis, float degree) {
	if (ID < objects.size()) {
		objects[ID].rotate(axis, degree);
	}

}
