#include "ComponentManager.h"



ComponentManager::ComponentManager()
{

}


ComponentManager::~ComponentManager()
{

}


CamComponent ComponentManager::getCamera() {
	unsigned int index = 0;
	for each (CamComponent c in cameras) {
		if (c.camera.enabled) {
			return c;
		}
		index++;
	}
	CamComponent c;
	return c;
}

std::vector<PLightComponent> ComponentManager::getPointLights() {
	return pointLights;
}

std::vector<ModelComponent> ComponentManager::getModels() {
	return models;
}


unsigned int ComponentManager::addModel(ModelComponent mc) {
	models.push_back(mc);
	return models.size() - 1;
}

unsigned int ComponentManager::addCamera(CamComponent cam) {
	cameras.push_back(cam);
	return cameras.size() - 1;
}

unsigned int ComponentManager::addPointLight(PLightComponent pl) {
	pointLights.push_back(pl);
	return pointLights.size() - 1;
}

unsigned int ComponentManager::addCamera(Camera camera) {
	CamComponent result;
	result.camera = camera;
	result.componentTransform = Component();
	cameras.push_back(result);
	return cameras.size() - 1;
}

unsigned int ComponentManager::addPointLight(PointLight light) {
	PLightComponent result;
	result.light = light;
	result.componentTransform = Component();
	pointLights.push_back(result);
	return pointLights.size() - 1;
}

unsigned int ComponentManager::addModel(unsigned int ID) {
	ModelComponent result;
	result.model = ID;
	result.componentTransform = Component();
	models.push_back(result);
	return models.size() - 1;
}


void ComponentManager::deleteCamera(unsigned int ID) {
	try {
		cameras.erase(cameras.begin() + ID);
	}
	catch (std::exception ex) {
		std::cout << "An exception has occured." << std::endl;
	}
}


void ComponentManager::deletePointLight(unsigned int ID) {
	try {
		pointLights.erase(pointLights.begin() + ID);
	}
	catch (std::exception ex) {
		std::cout << "An exception has occured." << std::endl;
	}
}

void ComponentManager::deleteModel(unsigned int ID) {
	try {
		models.erase(models.begin() + ID);
	}
	catch (std::exception ex) {
		std::cout << "An exception has occured." << std::endl;
	}
}