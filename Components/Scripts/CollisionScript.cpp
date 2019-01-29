#include "CollisionScript.h"

CollisionScript::CollisionScript() {
	SetName("CollisionScript"); //DO NOT EDIT 
}

CollisionScript::~CollisionScript() {

}

void CollisionScript::OnStart() {
	
}

void CollisionScript::OnUpdate() {
	
}

float delta;

void CollisionScript::OnFixedUpdate(float deltaTime) {
	delta = deltaTime;
}

void CollisionScript::OnCollide(Collision c) {
	//SetLinearVelocity(-GetLinearVelocity() * 0.85f);

	glm::vec3 momentum = c.hit_linear_velocity * c.hit_mass; 
	momentum += GetMass() * GetLinearVelocity();

	momentum *= GetMass() / (c.hit_mass + GetMass());
	momentum /= GetMass();


	if (GetLinearVelocity().x / momentum.x <= 0) {
		SetLinearVelocity(momentum * 0.85f);
	}
	else {
		SetLinearVelocity(-momentum * 0.85f);
	}


	
}

void CollisionScript::OnRaycast(RayHit rc) {

}