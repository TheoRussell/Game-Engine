#include "ClientHandler.h"


ClientHandler::ClientHandler()
{
	if (engineView == PRODUCT) {
		InGame = true;
	}
}


float scrollWheel;
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	scrollWheel = (float)yoffset;
}

void ClientHandler::setScroll(GLFWwindow *window) {
	glfwSetScrollCallback(window, scroll_callback);
}

void ClientHandler::updateScroll() {
	ScrollData = scrollWheel;
	scrollWheel = 0.0f;
}

bool ClientHandler::loadProject() {
	if (WorkingDir == "") {
		std::cout << "There are no projects active, please load one and then return to Product mode!" << std::endl;
	}
	else {
		try {


			std::string projFilePath = "";

			std::string fileName;
			std::vector<std::string> files;
			//Finding all the file names in a directory.
			for (auto & p : std::experimental::filesystem::directory_iterator(WorkingDir)) {
				fileName = p.path().string();
				fileName.erase(0, WorkingDir.length());
				files.push_back(fileName);
				//Checking whether the file is a .proj (the engines project files).
				if (fileName.find(".proj") != std::string::npos) {
					projFilePath = fileName;
				}
			}
			//Requesting the project to be loaded.
			project.directory = WorkingDir;
			project.load(projFilePath);

			//Returns a boolean if the scene successfully loaded.
			return world.loadBinary(WorkingDir + "Scenes\\" + project.startScene);

		}
		catch (std::exception ex) {
			std::cout << ex.what() << std::endl;
		}
	}


	return false;
}

unsigned int preUpdate = 0;
double pps = 1 / 60; //Updates per second.
unsigned int keyDelay_g = 60;

unsigned int keyDelay_m = 60;

void ClientHandler::update(GLFWwindow *window)
{

	if (InGame) {
		InGamePreview = false;
	}
	if (InGamePreview) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else if (!InGame) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	if (closeProgram) {
		project.save();
		glfwSetWindowShouldClose(window, true);
	}
	else {
		CamComponent cameraData = world.getCamera(InGame);
		unsigned int currentTime = (unsigned int)glfwGetTime();
		if (currentTime - preUpdate >= pps) {
			keyDelay_g++; //View collisions key.
			keyDelay_m++; //Change transformation mode.
			preUpdate = currentTime;
			//Every 60th of a second.

			if (cameraData.camera.enabled) {
				if (InGamePreview) {
					//Interpolating camera positions.
					cameraData.camera.lerpTransform();
					cameraData.camera.lerpMouse(cameraData.componentTransform.yaw, cameraData.componentTransform.pitch);
					cameraData.camera.calcViewMatrix(cameraData.componentTransform.position);
				}
				else {
					//Involve position of scene object in camera interpolation for in the game.
					cameraData.camera.lerpTransform();
					cameraData.camera.lerpMouse(cameraData.componentTransform.yaw + world.getObject(cameraData.componentTransform.objID).yaw, cameraData.componentTransform.pitch + world.getObject(cameraData.componentTransform.objID).pitch);
					cameraData.camera.calcViewMatrix(cameraData.componentTransform.position + world.getObject(cameraData.componentTransform.objID).pos);
				}


				//Offline movements etc.
				mouse(window);
				cameraData.camera.r_newX = 0;
				cameraData.camera.r_newY = 0;
				//Generic movement.
				if (!cameraData.camera.locked) {
					if (mouse_xOffset > 0 || mouse_yOffset > 0 || mouse_xOffset < 0 || mouse_yOffset < 0) {
						//Transform the scene object if in the game, rather than just the camera in the preview.
						if (!viewingCursor && !InGamePreview) {
							world.objects[cameraData.componentTransform.objID].yaw = glm::mod(world.objects[cameraData.componentTransform.objID].yaw + (mouse_xOffset / 60.0f), 360.0f);
							world.objects[cameraData.componentTransform.objID].pitch += mouse_yOffset / 60.0f;
							if (world.objects[cameraData.componentTransform.objID].pitch > 89.0f) { world.objects[cameraData.componentTransform.objID].pitch = 89.0f; }
							if (world.objects[cameraData.componentTransform.objID].pitch < -89.0f) { world.objects[cameraData.componentTransform.objID].pitch = -89.0f; }
						}

						if (InGamePreview) {
							cameraData.componentTransform.yaw = glm::mod(cameraData.componentTransform.yaw + (mouse_xOffset / 60.0f), 360.0f);
							cameraData.componentTransform.pitch += mouse_yOffset / 60.0f;
							if (cameraData.componentTransform.pitch > 89.0f) { cameraData.componentTransform.pitch = 89.0f; }
							if (cameraData.componentTransform.pitch < -89.0f) { cameraData.componentTransform.pitch = -89.0f; }
						}
					}
				}
			}


			cameraData.camera.newPos = glm::vec3(0.0f, 0.0f, 0.0f);


			if (KeyDown(window, KEY_ESCAPE)) {
				//Exiting the small play window or leaving the test window.
				if (engineView != PRODUCT) {
					if (InGamePreview) {
						InGamePreview = false;
					}
					else if (InGame) {
						InGame = false;
					}
					if (engineView == Game) {
						engineView = Editor;
					}
				}
			}
			else if (KeyDown(window, KEY_G) && engineView != PRODUCT && engineView != UIEditor) {
				//Collisions
				if (keyDelay_g >= 10) {
					viewingCollisions = !viewingCollisions;
					keyDelay_g = 0;
				}
			}
			else if (KeyDown(window, KEY_M) && InGamePreview) {
				//Changing selected object transformation mode.
				if (keyDelay_m >= 10) {
					if (editMode == MOVE) {
						editMode = SCALE;
					}
					else if (editMode == SCALE) {
						editMode = ROTATE;
					}
					else if (editMode == ROTATE) {
						editMode = MOVE;
					}
					keyDelay_m = 0;
				}
			}


			if (InGamePreview) {
				if (KeyDown(window, KEY_W)) {
					//Forwards
					cameraData.componentTransform.position += (cameraData.camera.getFront() * 1.05f) / 60.0f;
				}
				if (KeyDown(window, KEY_S)) {
					//Backwards
					cameraData.componentTransform.position -= (cameraData.camera.getFront() * 1.05f) / 60.0f;
				}
				if (KeyDown(window, KEY_SPACE)) {
					//Up
					cameraData.componentTransform.position += (glm::vec3(0.0f, 1.0f, 0.0f) * 1.05f) / 60.0f;
				}
				if (KeyDown(window, KEY_LSHIFT)) {
					//Down
					cameraData.componentTransform.position -= (glm::vec3(0.0f, 1.0f, 0.0f) * 1.05f) / 60.0f;
				}
				if (KeyDown(window, KEY_A)) {
					//Left
					cameraData.componentTransform.position -= (glm::normalize(glm::cross(cameraData.camera.getFront(), glm::vec3(0.0f, 1.0f, 0.0f))) * 1.05f) / 60.0f;
				}
				if (KeyDown(window, KEY_D)) {
					//Right
					cameraData.componentTransform.position += (glm::normalize(glm::cross(cameraData.camera.getFront(), glm::vec3(0.0f, 1.0f, 0.0f))) * 1.05f) / 60.0f;
				}
			}

			if (!InGamePreview && InGame) {
				world.cachedGameCam = cameraData;
			}
			if (InGamePreview) {
				world.cachedEditorCam = cameraData;
			}


		}
	}
}


bool ClientHandler::KeyDown(GLFWwindow *window, int key) {
	try {
		return glfwGetKey(window, key) == GLFW_PRESS;
	}
	catch (std::exception ex) {
		return false;
	}
	return false;
}



void ClientHandler::mouse(GLFWwindow* window) {
	double xpos, ypos;

	//Mouse data will only be received if the cursor is embedded in program, not visible.
	if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
		//Calculates the offset of the mouse from its last position (centre of the screen).
		glfwGetCursorPos(window, &xpos, &ypos);
		mouse_xOffset = (float)xpos - lastX; //Used for rotating camera.
		mouse_yOffset = lastY - (float)ypos;
		lastX = (float)xpos;
		lastY = (float)ypos;
	}
	else {
		mouse_xOffset = 0;
		mouse_yOffset = 0;
	}

}


ClientHandler::~ClientHandler()
{

}
