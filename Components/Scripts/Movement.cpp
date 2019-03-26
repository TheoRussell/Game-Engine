#include "Movement.h"

Movement::Movement() {
	SetName("Movement"); //DO NOT EDIT 
}

Movement::~Movement() {
	DeleteStage();
}

void Movement::OnStart() {
	LoadStage("stage" + std::to_string(stageID) + ".txt");
	ModifyLabel("Level", std::to_string(stageID));
	ModifyLabel("Attempts", "#" + std::to_string(attempt));
	ResetStage();

}

void Movement::OnFixedUpdate(float deltaTime) {
	timer += deltaTime;
	
	if (!loadedStage) {
		OnStart();
		timer = -0.5f;
	}
	else if (timer > 0.0f) {
		ModifyLabel("Timer", std::to_string(timer) + "s");
		MovePlayer();
	}
	
}


void Movement::LoadStage(std::string path) {
	//Delete old stage.
	DeleteStage();
	//Load new stage.
	std::ifstream file = std::ifstream(path);
	if (file.good()) {
		std::string line;
		//Fetch x size.
		std::getline(file, line);
		map_height = std::atoi(line.c_str());
		//Fetch z size.
		std::getline(file, line);
		map_width = std::atoi(line.c_str());
		GenStage();

		int x = 0;
		int y = 0;
		while (std::getline(file, line)) {
			y = 0;
			for (char yType : line) {
				//Check if it is a special block.
				int asci = int(yType);
				if (asci >= 65 && asci <= 90) {
					//ie, a letter. Upper case = trigger (100+). Lower case = response (200+).
					int id = (100 + (asci - 65));
					stage[map_height - 1 - x][y] = (BlockType)id;
				}
				else if (asci >= 97 && asci <= 122) {
					//Lowercase.
					int id = (200 + (asci - 97));
					stage[map_height - 1 - x][y] = (BlockType)id;
				}
				else {
					//Any other block.
					int yValue = std::atoi(&yType);
					stage[map_height - 1 - x][y] = (BlockType)yValue;
				}

				y++;
			}
			if (y < map_width-1) {
				for (int i = y; i < map_width; i++) {
					stage[map_height - 1 - x][i] = Empty;
				}
			}
			x++;
		}
	}
	file.close();


	SpawnStage();
	loadedStage = true;
}


void Movement::GenStage() {
	//Creating a multi-dimensional array;.
	stage = new BlockType*[map_height];
	for (int i = 0; i < map_height; i++) {
		stage[i] = new BlockType[map_width];
	}
}

void Movement::SpawnStage() {
	//Spawning the tiles into the scene.
	for (int x = 0; x < map_height; x++) {
		for (int y = 0; y < map_width; y++) {


			if (stage[x][y] == Tile | stage[x][y] == Start) {
				SpawnActor("tile", { x * 2,0.0f, y * 2 }, { 1.0f,1.0f,1.0f }, 0.0f, 0.0f, 0.0f);
			}
			else if (stage[x][y] == Goal) {
				SpawnActor("goal", { x * 2,0.0f, y * 2 }, { 1.0f,1.0f,1.0f }, 0.0f, 0.0f, 0.0f);
			}
			else if (stage[x][y] >= 100 && stage[x][y] < 200) {
				SpawnActor("tile", { x * 2,0.0f, y * 2 }, { 1.0f,1.0f,1.0f }, 0.0f, 0.0f, 0.0f);
				SpawnActor("pad1", { x * 2,0.1f, y * 2 }, { 1.0f,1.0f,1.0f }, 0.0f, 0.0f, 0.0f);
			}
			if (stage[x][y] == Start) {
				SetPosition({ x * 2, 0.0f, y * 2 });
				playerX = x;
				playerY = y;
			}

		}
	}
}


void Movement::ResetStage() {
	//Resetting stage data.
	falling = false;
	shape = UP;
	SetRoll(0.0f);
	SetPitch(0.0f);
	SetYaw(0.0f);
	originalRot = glm::vec3(0.0f);
	rotating = false;
	SetPosition({ 0.0f, 0.0f, 2.0f });
	SetLinearVelocity({ 0.0f, 0.0f, 0.0f });
	SetAngularVelocity({ 0.0f, 0.0f, 0.0f });

	AddInterfaceReference("Moves");
	AddInterfaceReference("Timer");
	AddInterfaceReference("Attempts");
	AddInterfaceReference("Level");
	timer = 0.0;
	moves = 0;

	for (int x = 0; x < map_height; x++) {
		for (int z = 0; z < map_width; z++) {
			if (stage[x][z] == Start) {
				SetPosition({ x * 2, 0.0f, z * 2 });
				playerX = x;
				playerY = z;
			}

		}
	}
}


void Movement::DeleteStage() {
	delete[] stage;
}

//Modifying interface.
void Movement::ModifyLabel(std::string id, std::string text) {
	try {
		UILabel * lbl = dynamic_cast<UILabel*>(GetInterfacePointers()->at(id));
		if (lbl != nullptr) {
			lbl->text = text;
		}
	}
	catch (std::exception ex) {

	}
}

//Game outcomes.
int Movement::PlayerOverAir() {

	//Checks each of the tile connections from the cube and the map.
	//Returns the type of the tile.
	if (playerX < 0 | playerX >= map_height | playerY < 0 | playerY >= map_width) {
		return true;
	}
	else if (stage[playerX][playerY] == Empty | (stage[playerX][playerY] >= 200 && stage[playerX][playerY] < 300)) {
		return true;
	}
	else if (stage[playerX][playerY] >= 100 && stage[playerX][playerY] < 200 && shape == UP) {
		return stage[playerX][playerY];
	}
	else {
		if (shape == F_FRONT) {
			if (playerX + 1 < map_height) {
				if (stage[playerX + 1][playerY] == Empty) {
					return true;
				}
			}
			else {
				return true;
			}
		}
		if (shape == F_BACK) {
			if (playerX - 1 >= 0) {
				if (stage[playerX - 1][playerY] == Empty) {
					return true;
				}
			}
			else {
				return true;
			}
		}
		if (shape == F_LEFT) {
			if (playerY - 1 >= 0) {
				if (stage[playerX][playerY - 1] == Empty) {
					return true;
				}
			}
			else {
				return true;
			}
		}
		if (shape == F_RIGHT) {
			if (playerY + 1 < map_width) {
				if (stage[playerX][playerY + 1] == Empty) {
					return true;
				}
			}
			else {
				return true;
			}
		}
	}
	return false;
}

bool Movement::PlayerHasWon() {
	if (shape == UP && stage[playerX][playerY] == Goal) {
		return true;
	}
	return false;
}

void Movement::Failed() {
	//Restarts the stage.
	ChangeScene("StageUnique");
	loadedStage = false;
	attempt++;
	ModifyLabel("Moves", "0");
}

void Movement::ModifyTileVisible(int id) {
	for (int x = 0; x < map_height; x++) {
		for (int z = 0; z < map_width; z++) {
			//Checks if it's a powered tile.
			if (stage[x][z] >= 200 && stage[x][z] < 300) {
				if (stage[x][z] - 200 == id) {
					//Sets the stage block to be an ordinary tile - prevents repeat spawns of actor.
					stage[x][z] = Tile;
					SpawnActor("tile", { x * 2,0.0f, z * 2 }, { 1.0f,1.0f,1.0f }, 0.0f, 0.0f, 0.0f);
				}
			}
		}
	}
}


void Movement::MovePlayer() {

	if (glfwGetTime() - lastPress > 0.1) {
		//Each .1 seconds a key may be clicked.

		//Detecting if each move has finished.
		if (IsChangeComplete()) {
			rotating = false;
			SetAngularVelocity({ 0.0f, 0.0f, 0.0f });
			if (!falling) {
				SetRotations();
			}
			
			moves += 1;
			ModifyLabel("Moves", std::to_string(moves));
		}

		if (!rotating) {
			if (!falling) {
				//Allow movement only while the object is stationary
				//Check if a key is pressed to trigger the timer for the next button press.
				if ((KeyDown(KEY_W) | KeyDown(KEY_S) | KeyDown(KEY_A) | KeyDown(KEY_D))) {
					lastPress = glfwGetTime();
					originalRot = { GetRoll(), GetPitch(), GetYaw() };
					rotating = true;
				}

				//Receiving WASD input for moving character.
				if (!Left()) { //Each function has similar code however, different angles are affected.
					if (!Right()) {
						if (!Forward()) {
							if (Backward()) {
								playerX -= 1;
							}
						}
						else {
							playerX += 1;
						}
					}
					else {
						playerY += 1;
					}
				}
				else {
					playerY -= 1;
				}
			}

			//Detecting if player is over air.
			int playerBlock = PlayerOverAir();

			if (playerBlock == 1) {
				if (GetPosition().y < -15.0f) {
					Failed();
				}
				else {
					AddForce({ 0.0f, -500.0f, 0.0f });
					falling = true;
				}
			}
			else if (playerBlock >= 100) {
				ModifyTileVisible(playerBlock - 100);
			}
			else {
				if (PlayerHasWon()) {
					AddTorque({0.0f, 0.0f, 40.0f});
					AddForce({0.0f, 4.0f, 0.0f});
					stageID++;
					if (stageID == 3) {
						//Win sequence.
						ChangeScene("InterfaceScene");
						ChangeUI("Win");
						attempt = 1;
						stageID = 1;
						moves = 0;
					}
					else {
						ChangeScene("StageUnique");
						loadedStage = false;
					}

				}
			}
		}
	}
}


//Movement.
bool Movement::Left() {
	if ((shape == UP | shape == F_LEFT | shape == F_RIGHT) && KeyDown(KEY_A)) {
		rotateMove = true;

		AddTorque({ -rotationSpeed, 0.0f, 0.0f });
		if (shape == UP) {
			Move({ 0.0f, 1.0f, -1.0f });
			shape = F_LEFT;
		}
		else if (shape == F_RIGHT | shape == F_LEFT) {
			if (shape == F_RIGHT) {
				shape = F_LEFT;
				SetRotations();
			}
			else {
				playerY -= 1;
			}
			shape = UP;
			Move({ 0.0f, -1.0f, -1.0f });
		}
		

		return true;
	}
	else if ((shape == F_FRONT | shape == F_BACK) && KeyDown(KEY_A)) {
		rotateMove = false;
		Move({ 0.0f, 0.0f, -2.0f });
		AddTorque({ -rotationSpeed, 0.0f, 0.0f });


		return true;
	}

	return false;
}
bool Movement::Right() {
	if ((shape == UP | shape == F_LEFT | shape == F_RIGHT) && KeyDown(KEY_D)) {
		rotateMove = true;

		AddTorque({ rotationSpeed, 0.0f, 0.0f });
		if (shape == UP) {
			Move({ 0.0f, 1.0f, 1.0f });
			shape = F_RIGHT;
		}
		else if (shape == F_RIGHT | shape == F_LEFT) {
			if (shape == F_LEFT) {
				shape = F_RIGHT;
				SetRotations();
			}
			else {
				playerY += 1;
			}

			shape = UP;
			Move({ 0.0f, -1.0f, 1.0f });
		}
		

		return true;
	}
	else if ((shape == F_FRONT | shape == F_BACK) && KeyDown(KEY_D)) {
		rotateMove = false;
		Move({ 0.0f, 0.0f,  2.0f });
		AddTorque({ rotationSpeed, 0.0f, 0.0f });
		
		return true;
	}

	return false;
}
bool Movement::Forward() {
				 if ((shape == UP | shape == F_FRONT | shape == F_BACK) && KeyDown(KEY_W)) {
					 rotateMove = true;

					AddTorque({ 0.0f, -rotationSpeed, 0.0f });
					if (shape == UP) {
						Move({ 1.0f, 1.0f, 0.0f });
						shape = F_FRONT;
					}
					else if (shape == F_FRONT | shape == F_BACK) {
						if (shape == F_BACK) {
							shape = F_FRONT;
							SetRotations();

						}
						else {
							playerX += 1;
						}
						shape = UP;
						Move({ 1.0f, -1.0f, 0.0f });
					}

					
					return true;
				}
				else if ((shape == F_LEFT | shape == F_RIGHT) && KeyDown(KEY_W)) {
					rotateMove = false;
					Move({ 2.0f, 0.0f, 0.0f });
					if (shape == F_LEFT) {
						AddTorque({ 0.0f, 0.0f, rotationSpeed });
					}
					else {
						AddTorque({ 0.0f, 0.0f, -rotationSpeed });
					}

					return true;
				}

				return false;
}
bool Movement::Backward() {
				 if ((shape == UP | shape == F_FRONT | shape == F_BACK) && KeyDown(KEY_S)) {
					 rotateMove = true;

					AddTorque({ 0.0f, rotationSpeed, 0.0f });
					if (shape == UP) {
						Move({ -1.0f, 1.0f, 0.0f });
						shape = F_BACK;
					}
					else if (shape == F_FRONT | shape == F_BACK) {
						if (shape == F_FRONT) {
							shape = F_BACK;
							SetRotations();
						}
						else {
							playerX -= 1;
						}
						shape = UP;
						Move({ -1.0f, -1.0f, 0.0f });
					}

					
					return true;
				}
				else if ((shape == F_LEFT | shape == F_RIGHT) && KeyDown(KEY_S)) {
					rotateMove = false;
					Move({ -2.0f, 0.0f, 0.0f });
					if (shape == F_LEFT) {
						AddTorque({ 0.0f, 0.0f, -rotationSpeed });
					}
					else {
						AddTorque({ 0.0f, 0.0f, rotationSpeed });
					}

					return true;

				}

				return false;
}

bool Movement::IsChangeComplete() {
	glm::vec3 change = glm::abs(originalRot - glm::vec3(GetRoll(), GetPitch(), GetYaw()));
	if (change.x >= 90.0f | change.y >= 90.0f | change.z >= 90.0f) {
		return true;
	}
	return false;
}
void Movement::SetRotations() {
	if (shape == UP) {
		SetPitch(0.0f);
		SetRoll(0.0f);
		SetYaw(0.0f);
		if (rotateMove) { SetPosition({ GetPosition().x, 0.0f, GetPosition().z }); }
		
	}
	else if (shape == F_RIGHT) {
		SetRoll(-90.0f);
		SetPitch(0.0f);
		SetYaw(0.0f);
		if (rotateMove) { SetPosition({ GetPosition().x, 1.0f, GetPosition().z + 4 }); }
		
	}
	else if (shape == F_LEFT) {
		SetRoll(90.0f);
		SetPitch(0.0f);
		SetYaw(0.0f);
		if (rotateMove) { SetPosition({ GetPosition().x, 1.0f, GetPosition().z - 4 }); }
		
	}
	else if (shape == F_FRONT) {
		SetPitch(90.0f);
		SetRoll(0.0f);
		SetYaw(0.0f);
		if (rotateMove) { SetPosition({ GetPosition().x + 4, 1.0f, GetPosition().z }); }
		
	}
	else if (shape == F_BACK) {
		SetPitch(-90.0f);
		SetRoll(0.0f);
		SetYaw(0.0f);
		if (rotateMove) { SetPosition({ GetPosition().x - 4, 1.0f, GetPosition().z }); }
		
	}
	originalRot = { GetRoll(), GetPitch(), GetYaw() };
}
