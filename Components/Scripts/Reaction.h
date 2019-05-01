#pragma once
#include "ComponentScript.h"

class Reaction : public ComponentScript {
public:
	Reaction();
	~Reaction();
	void OnStart() override;
	void OnFixedUpdate(float deltaTime) override;
	void OnCollide(Collision collision) override;
private:

};