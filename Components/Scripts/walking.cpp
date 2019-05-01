#include "walking.h"

walking::walking() {
	SetName("walking"); //DO NOT EDIT 
}

walking::~walking() {

}

void walking::OnStart() {

}

void walking::OnFixedUpdate(float deltaTime) {
	if (KeyDown(KEY_A)) {
		Move(-GetRight() * deltaTime);
	}
	if (KeyDown(KEY_D)) {
		Move(GetRight()* deltaTime);
	}
	if (KeyDown(KEY_W)) {
		Move({ GetForward().x*deltaTime, 0.0f, GetForward().z*deltaTime });
	}
	if (KeyDown(KEY_S)) {
		Move({ -GetForward().x*deltaTime, 0.0f, -GetForward().z*deltaTime });
	}

	if (MouseDown(MOUSE_LEFT) && !mouseDown) {
		mouseDown = true;
		SpawnActor("Bullet", GetPosition() + glm::vec3(0.0f, 4.0f, 0.0f), { 1.0f, 1.0f, 1.0f }, GetRoll(), GetPitch(), GetYaw());
	}
	else if (!MouseDown(MOUSE_LEFT) && mouseDown) {
		mouseDown = false;
	}
}

