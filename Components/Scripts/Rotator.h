#pragma once
#include "ComponentScript.h"

class Rotator : public ComponentScript {
public:
	Rotator();
	~Rotator();
	void OnStart() override;
	void OnUpdate() override;
private:

};