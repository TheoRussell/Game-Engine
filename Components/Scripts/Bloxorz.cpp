#include "Bloxorz.h"

Bloxorz::Bloxorz() {
	SetName("Bloxorz"); //DO NOT EDIT 

	menu = Main;
	VariableAdd("menu", Main);
	VariableSet("menu", menu);

	VariableAdd("player", false);
	VariableSet("player", false);


	AudioStream::AddBuffer("music", AudioFile::generate("src\\sounds\\sci-fi_sound.wav"));
	music_source = AudioSource::generate();
	AudioStream::AddBuffer("button", AudioFile::generate("src\\sounds\\button.wav"));
}

Bloxorz::~Bloxorz() {
	//Appropriately clearing the sound from memory.
	AudioSource::remove(music_source);
	ClearClicks();
}

void Bloxorz::ClearClicks() {
	
	for (int id : buttonClickSources) {
		AudioSource::remove(id);
	}
	buttonClickSources.clear();
}


void Bloxorz::OnStart() {
	menu = (MenuView)VariableGetInt("menu");
	play_music = VariableGetBool("player");

	AddInterfaceReference("Play");
	AddInterfaceReference("MainMenu");
	AddInterfaceReference("ToggleMusic");
	AddInterfaceReference("Exit");
	AddInterfaceReference("Next");
	AddInterfaceReference("Skip");
	AddInterfaceReference("Back");


	AddInterfaceReference("Win");

	SetPitch(0.0f);
	SetRoll(0.0f);
	SetYaw(0.0f);
	SetCursorVisible(true);

	if (menu != Game) {
		ChangeUI("MainMenu");
		ChangeScene("InterfaceScene");
		
	}

}


void Bloxorz::UpdateMusic() {
	if (play_music) {
		if (AudioSource::checkState(music_source) != AL_PLAYING) {
			AudioSource::playAudio(music_source, AudioStream::GetBuffer("music"));
		}
	}
	else {
		AudioSource::stopAudio(music_source);
	}
	

}


void Bloxorz::OnFixedUpdate(float deltaTime) {

	UpdateMusic();
	InterfaceInteraction();

	VariableSet("menu", menu);
	VariableSet("player", play_music);

}


void Bloxorz::InterfaceInteraction() {
	if (ButtonPressed("MainMenu")) {
		ReturnToMenu();
	}

	if (menu == Game) {
		if (ButtonPressed("Win")) {
			ReturnToMenu();
		}
	}
	else if (menu == Main) {
		if (ButtonPressed("Play")) {
			ChangeUI("Instruction1");
			menu = Inst1;
		}

		if (ButtonPressed("ToggleMusic")) {
			play_music = !play_music;
		}

		if (ButtonPressed("Exit")) {
			ExitGame();
		}
	}
	else {
		if (menu >= Inst1 && menu <= Inst3) {
			int instrIndex = menu - Inst1 + 1;

			if (ButtonPressed("Back")) {
				ChangeUI("Instruction" + std::to_string(instrIndex - 1));
				menu = (MenuView)(menu - 1);
			}

			if (ButtonPressed("Next")) {
				if (instrIndex == 3) {
					Play();
				}
				else {
					ChangeUI("Instruction" + std::to_string(instrIndex + 1));
					menu = (MenuView)(menu + 1);
				}
			}

			if (menu == Inst1) {

				if (ButtonPressed("Skip")) {
					Play();
				}
			}
		}
	}
}

bool Bloxorz::ButtonPressed(std::string refID) {
	try {
		UIButton * button = dynamic_cast<UIButton*>(GetInterfacePointer(refID));
		if (button != nullptr) {
			if (button->Clicked) {
				button->Clicked = false;
				ButtonClick();
				return true;
			}
		}
	}
	catch (std::exception ex) {

	}

	return false;
}

void Bloxorz::ReturnToMenu() {
	ChangeUI("MainMenu");
	menu = Main;
	ChangeScene("InterfaceScene");
}

void Bloxorz::Play() {
	ChangeScene("StageUnique");
	ChangeUI("Game");
	menu = Game;
}

void Bloxorz::ButtonClick() {
	ClearClicks();

	int audioSource = AudioSource::generate();
	buttonClickSources.push_back(audioSource);
	AudioSource::playAudio(audioSource, AudioStream::GetBuffer("button"));
}