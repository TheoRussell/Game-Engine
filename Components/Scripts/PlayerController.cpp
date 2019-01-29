#include "PlayerController.h"

PlayerController::PlayerController() {
	SetName("PlayerController"); //DO NOT EDIT 

	AddInterfaceReference("Levels");
	AddInterfaceReference("MainMenu");

} 

PlayerController::~PlayerController() {

}

void PlayerController::OnStart() {
	
}


double timer = 0.0;
bool c_down = false;

void PlayerController::OnFixedUpdate(float deltaTime) {
	///Updates 30 frames per second.


	float multiplier = 4 * deltaTime;
	timer += deltaTime;

	//UIPBar* bar = dynamic_cast<UIPBar*>(GetInterfacePointers()->at("Test1"));
	//if (bar != nullptr) {
	//	bar->percent = 0.5f;

	//	bar->SetFillColour({ std::abs(std::sin(255.0f * glm::radians(timer))), std::abs(std::cos(255.0f * glm::radians(timer))), 0.0f, 255.0f });
	//}



	UIButton * bar = dynamic_cast<UIButton*>(GetInterfacePointers()->at("Levels"));
	if (bar != nullptr) {
		if (bar->Clicked) {
			bar->Clicked = false;
			ChangeUI("Levels");
		}
	}

	UIButton * bar2 = dynamic_cast<UIButton*>(GetInterfacePointers()->at("MainMenu"));
	if (bar2 != nullptr) {
		if (bar2->Clicked) {
			bar2->Clicked = false;
			ChangeUI("MainMenu");
		}
	}
	

	//DebugPrint(std::to_string(GetPitch()) + "," + std::to_string(GetRoll()) + "," + std::to_string(GetYaw()));

	///Movement.
	glm::vec3 forwardVec = GetForward();
	if (KeyDown(KEY_W)) {
		Move(multiplier * glm::vec3(forwardVec.x, 0.0f, forwardVec.z));
	}
	if (KeyDown(KEY_S)) { 
		Move(multiplier * glm::vec3(-forwardVec.x, 0.0f, -forwardVec.z));
	}
	if (KeyDown(KEY_A)) {
		Move(multiplier * -GetRight());
	}
	if (KeyDown(KEY_D)) {
		Move(multiplier * GetRight());
	}
	if (KeyDown(KEY_SPACE)) {
		Move(multiplier * glm::vec3(0.0f,1.0f,0.0f));
	}
	if (KeyDown(KEY_LSHIFT)) {
		Move(multiplier * glm::vec3(0.0f,-1.0f, 0.0f));
	}

	if (MouseDown(MOUSE_LEFT)) {
		Raycast(GetPosition(), GetForward(), 10.0f);
	}

	if (KeyDown(KEY_ESCAPE))	{
		ExitGame();
	}


	if (c_down && !KeyDown(KEY_C)) {
		SetCursorVisible(!GetCursorVisible());
		c_down = false;
	}

	if (KeyDown(KEY_C)) {
		c_down = true;
	}

}


void PlayerController::OnRaycast(RayHit data) {
	SetPosition(glm::vec3(data.collision.impact_position.x, 2.0f, data.collision.impact_position.z));
	
}

void PlayerController::OnCollide(Collision c) {

}


