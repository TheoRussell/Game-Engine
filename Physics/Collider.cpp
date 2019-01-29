#include "Collider.h"



Collider::Collider()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	pitch = 0.0f;
	yaw = 0.0f;
	roll = 0.0f;
}


Collider::~Collider()
{
}
