#pragma once

#define SCENE_H

#ifndef MAX_POINTLIGHTS
	#define MAX_POINTLIGHTS 100
#endif // !MAX_POINTLIGHTS




#ifndef H_BinaryFile
#include "BinaryFiles.h"
#endif

#ifndef OBJECT_H
#include "Object.h"
#endif


class Scene
{
public:
	Scene();
	~Scene();
	void defaultCamera();
	void deleteObjects();
	void disableCameras();
	CamComponent getCamera(bool inGame);
	CamComponent getCamera();
	void deleteScene();
	bool loadBinary(std::string WorkingDir);
	bool saveBinary(std::string WorkingDir);
	int getSelected();
	void deselectAll();
	unsigned int getObjectsCount();
	Object & getObject(unsigned int id);
	bool deleteObject(unsigned int id);
	std::vector<PLightComponent> getPointLights();
	void select(unsigned int ID);
	void deselect(unsigned int ID);
	bool isSelected(unsigned int ID);
	void setObject(unsigned int ID, Object & object);
	void setObjectScale(unsigned int ID, glm::vec3 scale);
	void setObjectPos(unsigned int ID, glm::vec3 pos);
	void setObjectRot(unsigned int ID, std::string axis, float angle);
	void scaleObject(unsigned int ID, glm::vec3 scale);
	void moveObject(unsigned int ID, glm::vec3 pos);
	void rotateObject(unsigned int ID, std::string axis, float degree);
	void deletePointLight(unsigned int ID);
	unsigned int getPLCount();
	void addPointLight(PointLight pl);
	std::vector<Object> getObjects();
	unsigned int addObject(Object o);
	unsigned int addObject(Object o, bool isActor);
	Scene interpolate(Scene other, double deltaTime);
	unsigned int addObject(Object o, std::string name, bool isActor);
	unsigned int addObject(Object o, std::string name);
	std::string name = "scene";
	std::vector<Object> objects;


	glm::vec3 voidCol = glm::vec3(0.0f);


	//Direction (sun) light.
	glm::vec3 d_ambient = glm::vec3(0.35f);
	glm::vec3 d_diffuse = glm::vec3(0.4f);
	glm::vec3 d_specular = glm::vec3(0.5f);

	bool cachedGame;
	CamComponent cachedEditorCam;
	CamComponent cachedGameCam;
private:
	unsigned int pointLightCount;
};

