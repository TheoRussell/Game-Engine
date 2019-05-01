#pragma once
#include "Scene.h"


//Ie, if the user is releasing the game, change the engine view variable into 5 = Product.
#define BUILD_MODE 0
#ifdef BUILD_MODE_PRODUCT
	#define BUILD_MODE 4
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
	PRODUCT
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
	bool KeyDown(GLFWwindow *window, int key);
	Camera mainCamera;
	Scene world;



	bool closeProgram = false;
	bool viewingCollisions = false;
	bool viewingCursor = true;
	EditMode editMode = MOVE;
	EngineView engineView = (EngineView)BUILD_MODE;


	Project project = Project();
	std::string WorkingDir = "";
	
	bool showUI = true;
	bool InGame = false;
	bool InGamePreview = false;

	
	float ScrollData;
	//Storing previous mouse position to change change.
	float lastX = 1080 / 2;
	float lastY = 720 / 2;
private:


	float mouse_xOffset;
	float mouse_yOffset;
};

