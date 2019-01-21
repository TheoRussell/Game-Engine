#include "PointLight.h"



PointLight::PointLight()
{
	active = true;
	glm::vec3 c = glm::vec3(1.0f, 1.0f, 1.0f);
	colour_diff = c;
	colour_amb = c;
	colour_spec = c;
}


PointLight::PointLight(glm::vec3 pos, glm::vec3 diffuse)
{
	glm::vec3 c = glm::vec3(1.0f, 1.0f, 1.0f);
	colour_diff = diffuse;
	colour_amb = c;
	colour_spec = c;
	
}

PointLight::PointLight(glm::vec3 diffuse)
{
	glm::vec3 c = glm::vec3(1.0f, 1.0f, 1.0f);
	colour_diff = diffuse;
	colour_amb = c;
	colour_spec = c;
}

PointLight::~PointLight()
{
}

bool PointLight::isActive() {
	return active;
}

void PointLight::setActive(bool a) {
	active = a;
}

void PointLight::setDiffuse(glm::vec3 c) {
	colour_diff = c;
}

void PointLight::setAmbient(glm::vec3 c) {
	colour_amb = c;
}

void PointLight::setSpecular(glm::vec3 c) {
	colour_spec = c;
}

glm::vec3 PointLight::getDiffuse() {
	return colour_diff;
}

glm::vec3 PointLight::getSpecular() {
	return colour_spec;
}

glm::vec3 PointLight::getAmbient() {
	return colour_amb;
}