#include "Player.h"

Player::Player() {
	SetName("Player"); //DO NOT EDIT 
}

Player::~Player() {

}


bool down = false;

bool flatRight = false;
bool flatNorth = false;


void Player::OnStart() {
	SetPosition({0.0f, 2.990f, -0.2f});
	flatRight = false;
	flatNorth = false;
}


void Player::OnUpdate() {


	if (VariableGetInt("timer") >= 10) {
		VariableSet("timer", (int)1);
		down = false;
	}
	
	double half = 180;


	std::cout << flatRight << "," << flatNorth << std::endl;

	if (KeyDown(KEY_RIGHT) && !down) {
		SetRoll(GetRoll() + 90.0f);
		flatRight = !flatRight;

		if (!flatNorth && !flatRight) {
			Move({ 0.0f, 1.0f, 0.0f });
		} else if (!flatNorth && flatRight) {
			Move({ 0.0f, -1.0f, 0.0f });
		}

		if (flatNorth) {
			Move({ 0.0f, 0.0f, 2.0f });
		}
		else {
			Move({ 0.0f, 0.0f, 3.0f });
		}
		
		down = true;
	} else if (KeyDown(KEY_LEFT) && !down) {
		
		SetRoll(GetRoll() - 90.0f);
		flatRight = !flatRight;

		if (!flatNorth && !flatRight) {
			Move({ 0.0f, 1.0f, 0.0f });
		}
		else if (!flatNorth && flatRight) {
			Move({ 0.0f, -1.0f, 0.0f });
		}

		if (flatNorth) {
			Move({ 0.0f, 0.0f, -2.0f });
		}
		else {
			Move({ 0.0f, 0.0f, -3.0f });
		}
		down = true;
	}
	else if (KeyDown(KEY_UP) && !down) {

		if (!flatRight) {
			if (flatNorth) {
				SetPitch(GetPitch() - 90.0f);
				Move({ 0.0f, 1.0f, 0.0f });
			}
			else {
				SetPitch(GetPitch() + 90.0f);
				SetYaw(GetYaw() - 90.0f);
				SetRoll(GetRoll() - 90.0f);
				Move({ 0.0f, -1.0f, 0.0f });
			}
			Move({ 3.0f, 0.0f, 0.0f });
			flatNorth = !flatNorth;
		}
		else {
			if (!flatNorth) {
				SetYaw(GetYaw() + 90.0f);
				Move({ 2.0f, 0.0f, 0.0f });
				//Moving when lying sideways (to right / left) and remaining 2 wide, 1 high;
			}
			else {
				SetYaw(GetYaw() + 90.0f);
				if (modf((double)GetYaw(), &half) == 0) {
					Move({ 2.0f, 1.0f, 0.0f });
				}
				else {
					Move({ 2.0f, -1.0f, 0.0f });
				}
			}
		}
		down = true;

	}
	else if (KeyDown(KEY_DOWN) && !down) {

		if (!flatRight) {
			SetPitch(GetPitch() + 90.0f);
			if (flatNorth) {
				Move({ 0.0f, 1.0f, 0.0f });
			}
			else {
				Move({ 0.0f, -1.0f, 0.0f });
			}
			Move({ -3.0f, 0.0f, 0.0f });
			flatNorth = !flatNorth;
		}
		else {
			if (!flatNorth) {
				SetYaw(GetYaw() - 90.0f);
				Move({ -2.0f, 0.0f, 0.0f });
				//Moving when lying sideways (to right / left) and remaining 2 wide, 1 high;
			}
			else {
				SetYaw(GetYaw() - 90.0f);
				if (modf((double)GetYaw(), &half) == 0) {
					Move({ -2.0f, 1.0f, 0.0f });
				}
				else {
					Move({ -2.0f, -1.0f, 0.0f });
				}
			}
		}
		down = true;

	}

	VariableAdd("timer", (int)1);
}

