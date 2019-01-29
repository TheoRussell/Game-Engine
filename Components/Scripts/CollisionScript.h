#pragma once
#include "ComponentScript.h"
#include <iostream>

class CollisionScript : public ComponentScript {
public:
	CollisionScript();
	~CollisionScript();
	void OnStart() override;
	void OnUpdate() override;
	void OnFixedUpdate(float deltaTime) override;
	void OnCollide(Collision collision) override;
	void OnRaycast(RayHit rc) override;
};