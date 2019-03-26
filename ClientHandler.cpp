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
			project.directory = WorkingDir;
			project.load(projFilePath);


			return world.loadBinary(WorkingDir + "Scenes\\" + project.startScene);

		}
		catch (std::exception ex) {
			std::cout << ex.what() << std::endl;
		}
	}


	return false;
}

unsigned int preUpdate = 0;
double pps = 1 / 120; //Packets per second.
unsigned int keyDelay_g = 60;
unsigned int keyDelay_p = 60;
unsigned int keyDelay_c = 60;
unsigned int keyDelay_cr = 60;
unsigned int keyDelay_r = 60;
unsigned int keyDelay_m = 60;

void ClientHandler::update(GLFWwindow *window)
{

	if (InGame) {
		InEditor = false;
	}
	if (InEditor) {
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
		//Sending data to the server.
		unsigned int currentTime = (unsigned int)glfwGetTime();
		if (currentTime - preUpdate >= pps) {
			keyDelay_g++; //Collider box wait
			keyDelay_p++; //Mouse wait
			keyDelay_c++; //Click wait
			keyDelay_cr++;
			keyDelay_m++;
			preUpdate = currentTime;
			//Every 60th of a second.

			if (cameraData.camera.enabled) {
				if (InEditor) {
					cameraData.camera.lerpTransform();
					cameraData.camera.lerpMouse(cameraData.componentTransform.yaw, cameraData.componentTransform.pitch);
					cameraData.camera.calcViewMatrix(cameraData.componentTransform.position);
				}
				else {
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

						if (!viewingCursor && !InEditor) {
							world.objects[cameraData.componentTransform.objID].yaw = glm::mod(world.objects[cameraData.componentTransform.objID].yaw + (mouse_xOffset / 60.0f), 360.0f);
							world.objects[cameraData.componentTransform.objID].pitch += mouse_yOffset / 60.0f;
							if (world.objects[cameraData.componentTransform.objID].pitch > 89.0f) { world.objects[cameraData.componentTransform.objID].pitch = 89.0f; }
							if (world.objects[cameraData.componentTransform.objID].pitch < -89.0f) { world.objects[cameraData.componentTransform.objID].pitch = -89.0f; }
						}

						if (InEditor) {
							cameraData.componentTransform.yaw = glm::mod(cameraData.componentTransform.yaw + (mouse_xOffset / 60.0f), 360.0f);
							cameraData.componentTransform.pitch += mouse_yOffset / 60.0f;
							if (cameraData.componentTransform.pitch > 89.0f) { cameraData.componentTransform.pitch = 89.0f; }
							if (cameraData.componentTransform.pitch < -89.0f) { cameraData.componentTransform.pitch = -89.0f; }
						}
					}
				}


				if (scrollWheel != 0) {
					unsigned int selectedID = world.getSelected();
					if (selectedID > -1) {
						if (InEditor) {
							if (glfwGetKey(window, ic.key_arrow_down) == GLFW_PRESS) {
								if (editMode == MOVE)
									world.moveObject(selectedID, glm::vec3(0.0f, 0.05f * scrollWheel, 0.0f));
								if (editMode == SCALE)
									world.scaleObject(selectedID, glm::vec3(0.0f, 0.05f * scrollWheel, 0.0f));
								if (editMode == ROTATE)
									world.rotateObject(selectedID, "y", 1.0f * scrollWheel);
							}
							if (glfwGetKey(window, ic.key_arrow_up) == GLFW_PRESS) {
								if (editMode == MOVE)
									world.moveObject(selectedID, glm::vec3(0.05f * scrollWheel, 0.0f, 0.0f));
								if (editMode == SCALE)
									world.scaleObject(selectedID, glm::vec3(0.05f * scrollWheel, 0.0f, 0.0f));
								if (editMode == ROTATE)
									world.rotateObject(selectedID, "x", 1.0f * scrollWheel);
							}
							if (glfwGetKey(window, ic.key_arrow_right) == GLFW_PRESS) {
								if (editMode == MOVE)
									world.moveObject(selectedID, glm::vec3(0.0f, 0.0f, 0.05f * scrollWheel));
								if (editMode == SCALE)
									world.scaleObject(selectedID, glm::vec3(0.0f, 0.0f, 0.05f * scrollWheel));
								if (editMode == ROTATE)
									world.rotateObject(selectedID, "z", 01.0f * scrollWheel);
							}
						}
					}


					scrollWheel = 0;
				}
			}


			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {

				if (keyDelay_c >= 10) {
					click_l = !click_l;
					keyDelay_c = 0;
				}

			}
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
				//
				if (keyDelay_cr >= 10) {
					click_r = !click_r;
					keyDelay_cr = 0;
				}
			}
			cameraData.camera.newPos = glm::vec3(0.0f, 0.0f, 0.0f);
			for (unsigned int keyID = 0; keyID < 348; keyID++) {
				try {
					//Detects key press.
					if (glfwGetKey(window, keyID) == GLFW_PRESS) {
						if (InEditor) {

							//Only allow input if the user isn't using the user interface.
							if (keyID == ic.key_forward || (!viewingCursor && keyID == ic.key_arrow_up)) {
								//Forwards
								cameraData.componentTransform.position += (cameraData.camera.getFront() * 1.05f) / 60.0f;
							}
							else if (keyID == ic.key_backward || (!viewingCursor && keyID == ic.key_arrow_down)) {
								//Backwards
								cameraData.componentTransform.position -= (cameraData.camera.getFront() * 1.05f) / 60.0f;
							}
							else if (keyID == ic.key_up) {
								//Up
								cameraData.componentTransform.position += (glm::vec3(0.0f, 1.0f, 0.0f) * 1.05f) / 60.0f;
							}
							else if (keyID == ic.key_down) {
								//Down
								cameraData.componentTransform.position -= (glm::vec3(0.0f, 1.0f, 0.0f) * 1.05f) / 60.0f;
							}
							else if (keyID == ic.key_left || (!viewingCursor && keyID == ic.key_arrow_left)) {
								//Left
								cameraData.componentTransform.position -= (glm::normalize(glm::cross(cameraData.camera.getFront(), glm::vec3(0.0f, 1.0f, 0.0f))) * 1.05f) / 60.0f;
							}
							else if (keyID == ic.key_right || (!viewingCursor && keyID == ic.key_arrow_right)) {
								//Right
								cameraData.componentTransform.position += (glm::normalize(glm::cross(cameraData.camera.getFront(), glm::vec3(0.0f, 1.0f, 0.0f))) * 1.05f) / 60.0f;
							}
						}

						if (keyID == ic.key_escape) {
							//Escape
							//closeProgram = true;
							if (engineView != PRODUCT) {
								if (InEditor) {
									InEditor = false;
								}
								else if (InGame) {
									InGame = false;
								}
								if (engineView == Game) {
									engineView = Editor;
								}
							}


						}
						else if (keyID == ic.key_viewCollisions && engineView != PRODUCT && engineView != UIEditor) {
							//Collisions
							if (keyDelay_g >= 10) {
								viewingCollisions = !viewingCollisions;
								keyDelay_g = 0;
							}
						}
						else if (keyID == 77 && InEditor) { //M
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
						else {
							//std::cout << keyID << std::endl;
						}
					}
				}
				catch (std::exception ex) {

				}
			}
			if (!InEditor && InGame) {
				world.cachedGameCam = cameraData;
			}
			if (InEditor) {
				world.cachedEditorCam = cameraData;
			}

			player.pos = cameraData.componentTransform.position + world.objects[cameraData.componentTransform.objID].pos;
			player.roll = mainCamera.getRoll();
			player.pitch = mainCamera.getPitch();
			player.yaw = mainCamera.getYaw();




		}
	}


	
}

void ClientHandler::mouse(GLFWwindow* window) {
	double xpos, ypos;

	//Mouse data will only be received if the cursor is embedded in program, not visible.
	if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
		glfwGetCursorPos(window, &xpos, &ypos);
		mouse_xOffset = (float)xpos - lastX;
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
