#pragma once
#include "ComponentScript.h"

class PlayerController : public ComponentScript {
public:
	PlayerController();
	~PlayerController();
	void OnStart() override;
	void OnFixedUpdate(float deltaTime) override;
	void OnRaycast(RayHit data) override;
	void OnCollide(Collision c) override;

private:

};