#pragma once

#define OBJECT_H

#include <map>
#include "Physics\PhysicsBody.h"

#ifndef SCRIPT_H
#include "Components\Scripts\ComponentScript.h"
#endif





class Object
{
public:

	Object operator-(const Object & other);


	Object operator*(const double & deltaTime);

	Object();
	Object(glm::vec3 p);
	Object(unsigned int id);
	Object(glm::vec3 _pos, glm::vec3 scale, float _pitch, float _yaw, float _roll);

	void GetScriptData(ComponentScript * script, std::string id);
	void PrepareScript(ComponentScript * script, std::string id, GLFWwindow * _window);

	std::vector<std::string> getDebugText(ComponentScript * script, std::string scriptID, GLFWwindow * _window);


	void startScript(ComponentScript * script, std::string scriptID, GLFWwindow * _window);
	void OnStart(ComponentScript * script, std::string scriptID, GLFWwindow * _window);
	void OnFixedUpdate(ComponentScript * script, std::string scriptID, GLFWwindow * _window, float deltaTime);
	void OnCollide(Collision c, ComponentScript * script, std::string scriptID, GLFWwindow * _window);
	void OnRaycast(RayHit rc, ComponentScript * script, std::string scriptID, GLFWwindow * _window);
	~Object();
	unsigned int addCamera();
	unsigned int addCamera(Camera camera);
	unsigned int addPointLight();
	unsigned int addPointLight(PointLight light);
	unsigned int addModel();
	unsigned int addModel(unsigned int ID);
	void select();
	void deselect();
	bool isSelected();
	Transform getTransformStruct();
	void addScript(std::string ID);
	void deleteScript(std::string ID);
	float distanceTo(glm::vec3 position);
	glm::mat4 getTransform();
	glm::vec3 GetForward();
	glm::vec3 getRight();
	glm::vec3 getPos();
	void rotate(std::string axis, float degree);
	void move(glm::vec3 amount);
	void scale(glm::vec3 scalars);
	void setScale(glm::vec3 scale);
	void setPos(glm::vec3 _pos);
	void setRot(std::string axis, float angle);
	void moveTo(glm::vec3 p);

	glm::vec3 pos;
	glm::vec3 sca;
	float pitch;
	float yaw;
	float roll;
	PhysicsBody physicsBody;
	std::string name = "obj";


	std::vector<std::string> componentScriptIDs;
	std::map<std::string, DataStorage> componentScriptData;

	std::vector<glm::vec3> forces;
	std::vector<glm::vec3> torques;


	ComponentManager components;
	std::vector<RayHit> raycasts;

	std::vector<Actor> newActors;
	bool deleteSelf = false;
	bool exitGame = false;

	std::string newScene = "";
	std::string newUI = "";
private:
	bool selected;
};

