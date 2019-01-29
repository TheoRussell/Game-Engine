#pragma once
#include "ComponentScript.h"

class DebugMsg : public ComponentScript {
public:
	DebugMsg();
	~DebugMsg();
	void OnStart() override;
	void OnUpdate() override;
private:

};