#include "BoxCollider.h"


BoxCollider::BoxCollider()
{
	defaultBox();
}


BoxCollider::~BoxCollider()
{
}

void BoxCollider::defaultBox() {
	maxX = 1;
	minX = -1;
	maxY = 1;
	minY = -1;
	maxZ = 1;
	minZ = -1;
	corners.clear();
	corners.push_back(glm::vec3(0.5f, 0.5f, 1.0f));
	corners.push_back(glm::vec3(0.5f, 0.5f, -1.0f));
	corners.push_back(glm::vec3(1.0f, 0.5f, 0.5f));
	corners.push_back(glm::vec3(-1.0f, 0.5f, 0.5f));
	corners.push_back(glm::vec3(0.5f, 1.0f, 0.5f));
	corners.push_back(glm::vec3(0.5f, -1.0f, 0.5f));

	corners.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	corners.push_back(glm::vec3(1.0f, 1.0f, -1.0f));
	corners.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
	corners.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
	corners.push_back(glm::vec3(1.0f, -1.0f, 1.0f));
	corners.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
	corners.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
	corners.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
}

void BoxCollider::translate(Transform t, BoxCollider &bc) {
	bc.maxX *= t.scale.x * scale.x;
	bc.minX *= t.scale.x * scale.x;
	bc.maxY *= t.scale.y * scale.y;
	bc.minY *= t.scale.y * scale.y;
	bc.maxZ *= t.scale.z * scale.z;
	bc.minZ *= t.scale.z * scale.z;

	bc.maxX += t.position.x + position.x;
	bc.minX += t.position.x + position.x;
	bc.maxY += t.position.y + position.y;
	bc.minY += t.position.y + position.y;
	bc.maxZ += t.position.z + position.z;
	bc.minZ += t.position.z + position.z;
	unsigned int index = 0;
	for each (glm::vec3 c in bc.corners) {
		c *= t.scale * scale;
		c += t.position + position;
		bc.corners[index] = c;
		index++;
	}
}