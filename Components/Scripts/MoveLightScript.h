#pragma once
#include "ComponentScript.h"

class MoveLightScript : public ComponentScript {
public:
	MoveLightScript();
	~MoveLightScript();
	void OnStart() override;
	void OnFixedUpdate(float deltaTime) override;
private:

};