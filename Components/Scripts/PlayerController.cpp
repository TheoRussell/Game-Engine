#include "PlayerController.h"

PlayerController::PlayerController() {
	SetName("PlayerController"); //DO NOT EDIT 

	AddInterfaceReference("Levels");
	AddInterfaceReference("MainMenu");

} 

PlayerController::~PlayerController() {

}

void PlayerController::OnStart() {
	AudioStream::AddBuffer("temp", AudioFile::generate("src\\sounds\\carMoving.wav"));
	source = AudioSource::generate();
}





void PlayerController::OnFixedUpdate(float deltaTime) {
	///Updates 30 frames per second.


	float multiplier = 4 * deltaTime;
	timer += deltaTime;


	if (AudioSource::checkState(source) != AL_PLAYING) {
		AudioSource::playAudio(source, AudioStream::GetBuffer("car"));
	}



	if (KeyDown(KEY_T)) {
		AudioSource::playAudio(source, AudioStream::GetBuffer("temp"));
		
	}


	UIButton * bar = dynamic_cast<UIButton*>(GetInterfacePointers()->at("Levels"));
	if (bar != nullptr) {
		if (bar->Clicked) {
			bar->Clicked = false;
			ChangeUI("Levels");

			int audioSource = AudioSource::generate();
			AudioSource::playAudio(audioSource, AudioStream::GetBuffer("temp"));
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


