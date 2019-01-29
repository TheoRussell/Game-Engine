#pragma once
#include "ComponentScript.h"

class GamePlayer : public ComponentScript {
public:
	GamePlayer();
	~GamePlayer();
	void OnStart() override;
	void OnUpdate() override;
	void OnFixedUpdate(float deltaTime) override;
	void OnCollide(Collision collision) override;
private:

};