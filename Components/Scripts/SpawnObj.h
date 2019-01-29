#pragma once
#include "ComponentScript.h"

class SpawnObj : public ComponentScript {
public:
	SpawnObj();
	~SpawnObj();
	void OnStart() override;
	void OnUpdate() override;
private:

};