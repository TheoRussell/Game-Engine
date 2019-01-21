#pragma once

#include "GLM/gtc/matrix_transform.hpp"
#include "GLM/gtc/type_ptr.hpp"

class PointLight
{
public:
	PointLight();
	PointLight(glm::vec3 pos, glm::vec3 diffuse);
	PointLight(glm::vec3 diffuse);
	~PointLight();
	bool isActive();
	void setActive(bool a);
	void setDiffuse(glm::vec3 c);
	void setAmbient(glm::vec3 c);
	void setSpecular(glm::vec3 c);
	glm::vec3 getDiffuse();
	glm::vec3 getSpecular();
	glm::vec3 getAmbient();

	bool active;
	glm::vec3 colour_diff;
	glm::vec3 colour_spec;
	glm::vec3 colour_amb;
	float linear = 0.09f;
	float quadratic = 0.032f;
private:
};

