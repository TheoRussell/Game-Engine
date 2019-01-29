#include "SpawnObj.h"

SpawnObj::SpawnObj() {
	SetName("SpawnObj"); //DO NOT EDIT 
}

SpawnObj::~SpawnObj() {

}

void SpawnObj::OnStart() {
}

void SpawnObj::OnUpdate() {
	Move(GetForward());
	VariableAdd("timer", 1);
	if (VariableGetInt("timer") > 40.0f) {
		Destroy();
	}
}

