#pragma once

#include "AudioEngine\AudioEngine.h"
#include <vector>


class PhysicsEngine
{
public:
	PhysicsEngine();
	~PhysicsEngine();
	void updateKeys();
	void start(Scene & scene, GLFWwindow * _window);
	void OnStart(Scene & scene, GLFWwindow * _window);
	void ChangeScene(Scene & scene, std::string name);
	void update(Scene & scene, std::string PROJpath);
	void update(Scene & scene, float deltaTime, std::string PROJpath);
	void load(std::string name, Scene & scene);
	std::string addScript(ComponentScript * script);
	Collision RayCast(Scene & scene, Object obj, float distance, float increments, glm::vec3 origin, glm::vec3 direction);
	Collision RayCast(Scene & scene, Object obj);
	Collision checkLineCollision(LineCollider & line, BoxCollider & box);
	Collision checkBoxCollision(BoxCollider & box1, BoxCollider & box2);
	glm::vec3 findMid(std::vector<glm::vec3> &corners);
	glm::vec3 getBiggestXZ(std::vector<glm::vec3>& corners);
	glm::vec3 getLowestXZ(std::vector<glm::vec3>& corners);
	Collision checkBoxCollision(BoxCollider & box1, BoxCollider & box2, float roll, float pitch, float yaw);


	std::map<std::string, ComponentScript*> scripts;

	GLFWwindow * window;
	bool exitGame = false;



	bool newActorRequest = false;
	std::string newUI = "";
private:


	float multiplier = 1.0f / 60;
};

