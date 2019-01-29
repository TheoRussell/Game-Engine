#include "LineCollider.h"



LineCollider::LineCollider()
{
	origin = glm::vec3(0.0f, 0.0f, 0.0f);
	direction = glm::vec3(0.0f, 1.0f, 0.0f);
	distance = 5.0f;
	increments = 0.1f;
}


LineCollider::~LineCollider()
{
}
