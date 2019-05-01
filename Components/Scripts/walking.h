#pragma once
#include "ComponentScript.h"

class walking : public ComponentScript {
public:
	walking();
	~walking();
	void OnStart() override;
	void OnFixedUpdate(float deltaTime) override;
private:
	bool mouseDown = false;
};