#include "PhysicsBody.h"


PhysicsBody::PhysicsBody()
{
	doesGravity = false;
	collides = false;
}


PhysicsBody::~PhysicsBody()
{
}

void PhysicsBody::update(Transform t) {
	pitch = t.pitch;
	roll = t.roll;
	yaw = t.yaw;
	position = t.position;
	scale = t.scale;
}