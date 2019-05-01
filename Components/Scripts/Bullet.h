#pragma once
#include "ComponentScript.h"

class Bullet : public ComponentScript {
public:
	Bullet();
	~Bullet();
	void OnStart() override;
	void OnFixedUpdate(float deltaTime) override;
	void OnCollide(Collision collision) override;
private:

};