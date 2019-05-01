#pragma once

#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include "GLM/gtc/type_ptr.hpp"

class Collider
{
public:
	Collider();
	~Collider();
	glm::vec3 position;
	glm::vec3 scale;
	float pitch;
	float yaw;
	float roll;
};

