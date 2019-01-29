#pragma once
#include "ComponentScript.h"

class MissilePlayer : public ComponentScript {
public:
	MissilePlayer();
	~MissilePlayer();
	void OnStart() override;
	void OnUpdate() override;
private:

};