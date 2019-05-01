#include "Bullet.h"

Bullet::Bullet() {
	SetName("Bullet"); //DO NOT EDIT 

	NewVariable("timer", 0.0f);
}

Bullet::~Bullet() {

}

void Bullet::OnStart() {
	SetMass(5.0f);
}

void Bullet::OnFixedUpdate(float deltaTime) {
	SetLinearVelocity(GetForward()* 20.0f);
	VariableAdd("timer", deltaTime);
	if (VariableGetFloat("timer") >= 4.0) {
		Destroy();
	}
}

void Bullet::OnCollide(Collision collision)
{
	if (collision.hit_label == "crate") {
		SetLinearVelocity(-GetLinearVelocity());
	}
}

