#include "Rotator.h"

Rotator::Rotator() {
	SetName("Rotator"); //DO NOT EDIT 
}

Rotator::~Rotator() {

}

void Rotator::OnStart() {

}

void Rotator::OnUpdate() {
	float multiplier = 50.0f * (1.0f / 30.0f);
	SetYaw(GetYaw() + multiplier);
}

