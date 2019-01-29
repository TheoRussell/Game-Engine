#include "GamePlayer.h"

GamePlayer::GamePlayer() {
	SetName("GamePlayer"); //DO NOT EDIT 
}

GamePlayer::~GamePlayer() {

}

void GamePlayer::OnStart() {
	
}

void GamePlayer::OnUpdate() {

}

void GamePlayer::OnFixedUpdate(float deltaTime) {
	VariableSet("oldX", (float)GetPosition().x);
	VariableSet("oldY", (float)GetPosition().y);
	VariableSet("oldZ", (float)GetPosition().z);

	float multiplier = (1.0f / 30.0f) * 3;

	if (KeyDown(KEY_D)) {
		SetYaw(0.0f);
		Move(glm::vec3(1.0f, 0.0f, 0.0f) * multiplier);
	}
	if (KeyDown(KEY_A)) {
		SetYaw(180.0f);
		Move(glm::vec3(-1.0f, 0.0f, 0.0f) * multiplier);
	}

	if (VariableGetBool("jumping")) {
		int jumpTime = VariableGetInt("jumpTime");
		float moveHeight = -0.0009*(jumpTime - 15)*(jumpTime - 15)*(jumpTime - 15);
		Move(glm::vec3(0.0f, moveHeight, 0.0f) * multiplier);
		VariableAdd("jumpTime", (int)1);
		if (jumpTime > 29) {
			VariableSet("jumping", false);
		}
		VariableSet("onGround", false);
	}
	else {
		if (KeyDown(KEY_SPACE) && VariableGetBool("onGround")) {
			VariableSet("jumping", true);
			VariableSet("jumpTime", 0);
		}
		else {
			Move(-glm::vec3(0.0f, 1.0f * multiplier, 0.0f));
		}
	}


	if (VariableGetInt("spawnTimer") > 0) {
		VariableAdd("spawnTimer", (int)1);
	}
	if (VariableGetInt("spawnTimer") > 10) {
		VariableSet("spawnTimer", (int)0);
	}
	if (MouseDown(MOUSE_LEFT) && VariableGetInt("spawnTimer") == 0) {
		SpawnActor("actor1", GetPosition(), glm::vec3(0.2f), GetRoll(), GetPitch(), GetYaw());
		VariableAdd("spawnTimer", (int)1);
	}


}


void GamePlayer::OnCollide(Collision collision) {
	//setPos(glm::vec3(getVariableFloat("oldX"), getVariableFloat("oldY"), getVariableFloat("oldZ")));
	
	if (collision.b1_min.y > collision.b2_max.y - 0.2) {
		Move(glm::vec3(0.0f, collision.distanceY, 0.0f));
		VariableSet("onGround", true);
	}
	else {
		if (collision.distanceY > 0) {
			Move(glm::vec3(0.0f, collision.distanceY, 0.0f));
		}

		if (collision.objID != 0) Move(glm::vec3(collision.distanceX, 0.0f, 0.0f));
	}
}
