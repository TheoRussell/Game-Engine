#pragma once
#include "ComponentScript.h"

class Player : public ComponentScript {
public:
	Player();
	~Player();
	void OnStart() override;
	void OnUpdate() override;
private:
	
};