#include "Reaction.h"

Reaction::Reaction() {
	SetName("Reaction"); //DO NOT EDIT 
}

Reaction::~Reaction() {

}

void Reaction::OnStart() {

}

void Reaction::OnFixedUpdate(float deltaTime) {

}

void Reaction::OnCollide(Collision collision)
{
	if (collision.hit_label.find("Bullet") != std::string::npos) {
		AddForce(collision.hit_linear_velocity);
	}
}

