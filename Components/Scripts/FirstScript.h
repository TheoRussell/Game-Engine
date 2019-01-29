#pragma once
#include "ComponentScript.h"

#include <iostream>

class FirstScript : public ComponentScript
{
public:
	FirstScript();
	~FirstScript();
	void OnStart() override;
	void OnFixedUpdate(float deltaTime) override;
	void OnCollide(Collision collision) override;
private:
	float time = 0;
};

