#pragma once
#include "BoxCollider.h"
#include "LineCollider.h"


class PhysicsBody
{
public:
	PhysicsBody();
	~PhysicsBody();
	void update(Transform t);
	bool doesGravity = false;
	bool collides = false;
	std::vector<BoxCollider> coll_Box;
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	float yaw = 0.0f;
	float pitch = 0.0f;
	float roll = 0.0f;

	bool enabled = false;

	//Mass in kg.
	float mass = 50.0f;
	glm::vec3 linear_velocity = glm::vec3(0.0f);
	glm::vec3 angular_velocity = glm::vec3(0.0f);

private:

};

