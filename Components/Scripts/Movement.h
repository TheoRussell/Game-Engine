#pragma once
#include "ComponentScript.h"


enum block_rotation {
	UP,
	F_RIGHT,
	F_LEFT,
	F_FRONT,
	F_BACK
};


class Movement : public ComponentScript {
public:
	Movement();
	~Movement();
	void OnStart() override;
	void OnFixedUpdate(float deltaTime) override;
	void LoadStage(std::string path);
	void GenStage();
	void SpawnStage();
	void ResetStage();
	void DeleteStage();
	void ModifyLabel(std::string id, std::string text);
	int PlayerOverAir();
	bool PlayerHasWon();
	void SetRotations();
	void MovePlayer();
	void Failed();
	void ModifyTileVisible(int id);
	bool Left();
	bool Right();
	bool Forward();
	bool Backward();
	bool IsChangeComplete();
private:
	float lastPress = 0.0f;
	block_rotation shape = UP;
	bool rotating = false;
	bool rotateMove = false;
	float rotationSpeed = 10000.0f;
	glm::vec3 originalRot = glm::vec3(0.0f);


	double timer = 0.0;
	int moves = 0;
	
	int playerX;
	int playerY;
	bool falling = false;


	enum BlockType {
		Empty =0,
		Tile = 1,
		Goal = 2,
		Start = 3,
	};
	int map_width = 7;
	int map_height = 7;
	BlockType** stage = NULL;
	bool loadedStage = false;
	int stageID = 1;
	int attempt = 1;
};