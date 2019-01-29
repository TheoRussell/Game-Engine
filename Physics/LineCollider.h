#pragma once
#include "Physics\Collider.h"

class LineCollider :
	public Collider
{
public:
	LineCollider();
	~LineCollider();
	glm::vec3 origin = glm::vec3(0.0f,0.0f,0.0f);
	glm::vec3 direction = glm::vec3(0.0f,1.0f,0.0f);
	float distance = 5.0f;
	float increments = 0.1f;
};

