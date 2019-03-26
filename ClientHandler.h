#pragma once
#include "Scene.h"


//Ie, if the user is releasing the game, change the engine view variable into 5 = Product.
#define BUILD_MODE 0
#ifdef BUILD_MODE_PRODUCT
	#define BUILD_MODE 5
#endif


#include <vector>
#include <GLFW/glfw3.h>
#include "GLM/glm.hpp"
#include <thread>


#include "Camera.h"

#include "Project.h"



enum EngineView {
	Projects,
	Editor,
	Game,
	UIEditor,
	PUBLISHED,
	PRODUCT
};

struct ObjectTransform {
	bool newObject = false;
	unsigned int modelID = 0;
	unsigned int id = 0;
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	float yaw = 0;
	float pitch = 0;
	float roll = 0;
};

struct PlayerData {
	unsigned int id;
	glm::vec3 position;
	float yaw;
	float pitch;
	float roll;
};

struct InputControls {
	unsigned int key_forward = 87;
	unsigned int key_backward = 83;
	unsigned int key_left = 65;
	unsigned int key_right = 68;
	unsigned int key_up = 32;
	unsigned int key_down = 340;
	unsigned int key_escape = 256;
	unsigned int key_viewCollisions = 71;
	unsigned int key_viewCursor = 80;
	unsigned int key_arrow_up = 265;
	unsigned int key_arrow_down = 264;
	unsigned int key_arrow_right = 262;
	unsigned int key_arrow_left = 263;
};

enum EditMode {
	MOVE,
	SCALE,
	ROTATE
};


class ClientHandler
{
public:
	ClientHandler();
	void setScroll(GLFWwindow * window);
	bool loadProject();
	void updateScroll();
	~ClientHandler();
	void mouse(GLFWwindow *window);
	void update(GLFWwindow *window);
	Camera mainCamera;
	Object player;
	Scene world;
	std::vector<PlayerData> players;
	int userID = 0;
	bool connected = false;
	bool closeProgram = false;
	std::string version = "v1.0";
	InputControls ic;
	bool viewingCollisions = false;
	bool viewingCursor = true;
	bool click_l = false;
	bool click_r = false;
	EditMode editMode = MOVE;


	Project project = Project();
	std::string WorkingDir = "";
	EngineView engineView = (EngineView)BUILD_MODE;
	bool showUI = true;
	bool InGame = false;
	bool InEditor = false;

	
	float ScrollData;

	//Storing previous mouse position to change change.
	float lastX = 1080 / 2;
	float lastY = 720 / 2;
private:


	float mouse_xOffset;
	float mouse_yOffset;
};

