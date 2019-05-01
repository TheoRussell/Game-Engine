#pragma once
#include "ComponentScript.h"

class Bloxorz : public ComponentScript {
public:
	Bloxorz();
	~Bloxorz();
	void ClearClicks();
	void OnStart() override;
	void UpdateMusic();
	void OnFixedUpdate(float deltaTime) override;
	void InterfaceInteraction();
	bool ButtonPressed(std::string refID);
	void ReturnToMenu();
	void Play();
	void ButtonClick();
private:

	enum MenuView {
		Main,
		Stages,
		Game,
		Inst1,
		Inst2,
		Inst3
	};
	MenuView menu = Stages;
	int music_source;
	bool play_music = false;

	std::vector<int> buttonClickSources;

};