#include "MissilePlayer.h"

MissilePlayer::MissilePlayer() {
	SetName("MissilePlayer"); //DO NOT EDIT 
}

MissilePlayer::~MissilePlayer() {

}

void MissilePlayer::OnStart() {
	SetPosition(glm::vec3(0.0f,0.0f,-10.0f));
	VariableSet("score", (int)0);


}

void MissilePlayer::OnUpdate() {
	
	float multiplier = 1.0f * (1.0f/30.0f);
	Move({ 0.0f, 0.0f, multiplier });

	multiplier *= 3;
	if (KeyDown(KEY_A)) {
		Move({ multiplier, 0.0f, 0.0f });
	}
	if (KeyDown(KEY_D)) {
		Move({ -multiplier, 0.0f, 0.0f });
	}
	if (KeyDown(KEY_W)) {
		Move({ 0.0f, multiplier, 0.0f });
	}
	if (KeyDown(KEY_S)) {
		Move({ 0.0f, -multiplier, 0.0f });
	}
	
	if (((int)GetPosition().z % 10 == 0) && (abs((int)GetPosition().z - GetPosition().z)) < 0.05f) {
		VariableAdd("score", (int)1);
		SpawnActor("rotator", glm::vec3(GetPosition().x,GetPosition().y,GetPosition().z+10.0f), glm::vec3(1.0f),0.0f,0.0f,0.0f);
	}


	VariableAdd("bump", 0.01f);
	//getComponents().pointLights.at(0).light.setDiffuse(glm::vec3(0.0f,0.0f,getVariableFloat("bump")));
}

