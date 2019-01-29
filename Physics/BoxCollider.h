#pragma once
#include <vector>
#include "Physics\Collider.h"


struct Transform {
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	float yaw = 0;
	float pitch = 0;
	float roll = 0;
};

class BoxCollider : public Collider
{
public:
	BoxCollider();
	~BoxCollider();
	void defaultBox();
	void translate(Transform, BoxCollider &bc);
	float maxX;
	float minX;
	float maxY;
	float minY;
	float maxZ;
	float minZ;
	std::vector<glm::vec3> corners;
private:

};
