

#include "ResourceManager.h"
#include "Components\Scripts\ScriptManager.h"
#include "Components\ComponentManager.h"
#include "EngineInterface.h"

#include "GLM/gtc/matrix_transform.hpp"
#include "GLM/gtc/type_ptr.hpp"

#include <cstdlib> //Random number generation
#include <chrono> //Sleeping

#ifndef _audio_h_
#include "AudioEngine\AudioEngine.h"
#endif



//<@GENERATED CODE@SCRIPT IMPORT@>//
#include "Components\Scripts\FirstScript.h"
#include "Components\Scripts\CollisionScript.h"
#include "Components\Scripts\PlayerController.h"
#include "Components\Scripts\Player.h"
//<\@GENERATED CODE@SCRIPT IMPORT@>//


void framebuffer_size_callback(GLFWwindow* window, int width, int height);


unsigned int timeSinceScene = 0;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int width = 1920;
int height = 1080;


std::vector<Model> block_models;
std::vector<unsigned int> block_types;
std::vector<glm::vec3> block_positions;


CONST double render_update = 1.0 / 60.0;
CONST double physics_update = render_update * 2.0;



BOOL WINAPI HandlerRoutine(DWORD dwCtrlType) {

	return dwCtrlType == CTRL_CLOSE_EVENT;
}



void GLFWErrorHandling(int, const char* code) {
	//std::cout << "GLFW ERROR: " << code << std::endl;
}


int main(void)
{	
	std::cout << "OpenGL Development: Theodore Russell" << std::endl;
	std::cout << "Game Engine - V1.0" << std::endl;

	glfwSetErrorCallback(GLFWErrorHandling);

	/* Initialize the library */
	if (!glfwInit())
		return -1;


	//Multisampling
	//glfwWindowHint(GLFW_SAMPLES, 2);


	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow* window;
	window = glfwCreateWindow(width, height, "Game Engine", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
	
	//width = 1920;
	//height = 1080;
	int count;
	GLFWmonitor** monitors = glfwGetMonitors(&count);
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* video = glfwGetVideoMode(monitor);
	width = video->width;
	height = video->height;
	


	glfwSetWindowMonitor(window, monitor, 0, 0, width, height, video->refreshRate);
	glfwSetWindowSize(window, width, height);
	glfwSetWindowPos(window, 0, 0);

	glViewport(0, 0, width / 2, height / 2);

	if (glewInit() != GLEW_OK) {
		std::cout << "Error: not init" << std::endl;
		glfwTerminate();
		return -1;
	}

	//glEnable(GL_MULTISAMPLE);

	
	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsDark();


	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	ResourceManager resources;
	PhysicsEngine physics;
	AudioEngine audio;
	ClientHandler client;
	ScriptManager scripts;
	ComponentManager components;
	EngineInterface engine_interface;

	engine_interface.width = width;
	engine_interface.height = height;



	int LoadSound = AudioSource::generate();
	AudioSource::playAudio(LoadSound, AudioStream::GetBuffer("car"));


	if (client.closeProgram) {
		//End program.
		resources.end();
		glfwTerminate();
		exit(1);
	}
	if (!client.connected) {
		//What to do if not connected to server.
	}

	float lastX = width / 2;
	float lastY = height / 2;
	glfwSetCursorPos(window, lastX, lastY);


	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); /*Resizing the window. - Calls the function using the window context*/

	glEnable(GL_DEPTH_TEST); //To prevent panes from overlapping.


	glm::ortho(0.0f, (float)width, 0.0f, (float)height, 0.1f, 300.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(client.mainCamera.getFOV()), (float)width / (float)height, 0.01f, 300.0f); //The model positions relative to 1 and -1.

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //Hides cursor, records position.


	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	while (client.engineView == Projects && !client.closeProgram) { //Loading the projects page
		
		glClearColor(0, 0, 0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); //reset all of the depth planes.

		engine_interface.projects(client);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	unsigned int blueArrowID = 0;
	unsigned int greenArrowID = 0;
	unsigned int redArrowID = 0;
	unsigned int blueScaArrowID = 0;
	unsigned int greenScaArrowID = 0;
	unsigned int redScaArrowID = 0;
	unsigned int blueRotArrowID = 0;
	unsigned int greenRotArrowID = 0;
	unsigned int redRotArrowID = 0;
	unsigned int sphereID = 0;
	Model cube = Model("Cube", "src\\models\\block.obj");

	if (client.engineView == PRODUCT) {
		client.WorkingDir = client.project.getActive();
		client.loadProject();
	}
	else {
		//Reducing load time by loading these only if the user is in the engine rather than complete game.

		Model arrow_blue("BlueArrow", "src\\models\\arrow.obj");
		Model arrow_green("GreenArrow", &arrow_blue, "green_arrow.mtl");
		Model arrow_red("RedArrow", &arrow_blue, "red_arrow.mtl");
		Model arrow_scale_blue("ScaleBlueArrow", "src\\models\\ArrowScale.obj");
		Model arrow_scale_green("ScaleGreenArrow", &arrow_scale_blue, "green_arrow.mtl");
		Model arrow_scale_red("ScaleRedArrow", &arrow_scale_blue, "red_arrow.mtl");
		Model arrow_rotate_blue("RotBlueArrow", "src\\models\\ArrowRotate.obj");
		Model arrow_rotate_green("RotGreenArrow", &arrow_rotate_blue, "green_arrow.mtl");
		Model arrow_rotate_red("RotRedArrow", &arrow_rotate_blue, "red_arrow.mtl");
		blueArrowID = resources.add(&arrow_blue);
		greenArrowID = resources.add(&arrow_green);
		redArrowID = resources.add(&arrow_red);
		blueScaArrowID = resources.add(&arrow_scale_blue);
		greenScaArrowID = resources.add(&arrow_scale_green);
		redScaArrowID = resources.add(&arrow_scale_red);
		blueRotArrowID = resources.add(&arrow_rotate_blue);
		greenRotArrowID = resources.add(&arrow_rotate_green);
		redRotArrowID = resources.add(&arrow_rotate_red);
		Model sphere = Model("Sphere", "src\\models\\sphere.obj");
		sphereID = resources.add(&sphere);

	}


	
	//<@GENERATED CODE@SCRIPTS@>//
	FirstScript firstScript;
	physics.addScript(&firstScript);
	CollisionScript script_CollisionScript;
	physics.addScript(&script_CollisionScript);
	PlayerController script_PlayerController;
	physics.addScript(&script_PlayerController);
	Player script_Player;
	physics.addScript(&script_Player);
	//<\@GENERATED CODE@SCRIPTS@>//

	int frames = 0;
	bool InGame = false;


	resources.setViewScene(0);
	client.setScroll(window); //Allows mouse scroll wheel input.






	/* Loop until the user closes the window */
	physics.start(client.world, window);
	float timer = (float)glfwGetTime();
	bool tempInGame = InGame;

	double lastFrame = glfwGetTime();
	double updateLag = 0.0;

	engine_interface.loadInterface(client.WorkingDir + "Interfaces\\" + client.project.startInterface + ".gui", client, physics);


	while (!glfwWindowShouldClose(window))
	{


		if (client.engineView == PUBLISHED) {
			client.closeProgram = true;
		}

		tempInGame = InGame;

		double currentFrame = glfwGetTime();
		double frameDelay = currentFrame - lastFrame;
		lastFrame = currentFrame;
		updateLag += frameDelay;


		Scene lastScene = client.world;

		if (client.engineView == UIEditor) {
			resources.clearDisplay();
			client.updateScroll();
		}
		else {
			while (updateLag >= physics_update) {
				client.update(window);
				InGame = client.InGame;
				if (InGame) {
					engine_interface.OnUpdate(physics_update, client, physics, resources);


					physics.update(client.world, physics_update, client.project.directory);
					audio.update(client.world);
					if (physics.newUI != "") {
						engine_interface.loadInterface(client.WorkingDir + "interfaces\\" + physics.newUI + ".gui", client, physics);
						physics.newUI = "";
					}
					int index = 0;
					for each (Object o in client.world.objects) {
						bool addedActors = false;
						for each (Actor act in o.newActors) {
							unsigned int newObj = client.world.addObject(resources.getActor(act.name, &engine_interface.debug_messages), true);
							client.world.objects[newObj].pos += act.pos;
							client.world.objects[newObj].sca *= act.scale;
							client.world.objects[newObj].roll += act.roll;
							client.world.objects[newObj].pitch += act.pitch;
							client.world.objects[newObj].yaw += act.yaw;
							addedActors = true;
						}
						o.newActors.clear();
						client.world.objects[index] = o;
						index++;
						if (addedActors) {
							break;
						}
					}

				}

				if (physics.exitGame) {
					client.closeProgram = true;
				}

				updateLag -= physics_update;
			}





			if (!InGame) {
				if (client.click_l && !client.click_r && !client.viewingCursor) {
					Collision c;
					c.collided = false;
					c = physics.RayCast(client.world, client.player);
					if (c.collided) {
						//Successful raycast.
						client.world.select(c.objID);
					}
					client.click_l = false;
				}
				if (client.click_l && !client.click_r && client.viewingCursor) {
					client.click_l = false;
				}
				if (client.click_r && !client.click_l && client.viewingCursor) {
					client.click_r = false;
				}

				if (client.click_r && !client.click_l && !client.viewingCursor) {
					client.world.deselect(client.world.getSelected());
					client.click_r = false;
				}
			}



			
			double deltaTime = updateLag / physics_update;
			Scene interpolatedScene = client.world.interpolate(lastScene, deltaTime);

			resources.setMatrices(client.world.getCamera(InGame).camera.viewMatrix(), projection); //SHOULD USE INTERPOLATED CAMERA BUT CAUSES BUG.
			glViewport(engine_interface.xDisplacement, height - (9 * (width - engine_interface.xDisplacement) / 16) - engine_interface.UIEy, width - engine_interface.xDisplacement, 9 * (width - engine_interface.xDisplacement) / 16);
			

			if (client.engineView == PRODUCT | client.engineView == Game) {
				resources.render(interpolatedScene, client.world.getCamera(InGame).camera.getPos() + client.world.getCamera(InGame).componentTransform.position, true);
				
			}
			else {
				resources.render(interpolatedScene, client.world.getCamera(InGame).camera.getPos() + client.world.getCamera(InGame).componentTransform.position, false);
			}


			if (client.viewingCollisions && client.engineView != PRODUCT) {
				//Display collision rectangles.
				for (unsigned int i = 0; i < client.world.getObjectsCount(); i++) {
					Object p = client.world.getObject(i);
					if (p.physicsBody.collides) {
						for each (BoxCollider bc in p.physicsBody.coll_Box) {
							resources.renderModel(1, 2, cube, p.pos + bc.position, p.sca * bc.scale, 0.0f, 0.0f, 0.0f);
						}
					}
				}
			}


			
			//display selected object using arrows and modify transformations.
			if (client.engineView != PRODUCT && client.engineView != Game) {
				int selected = client.world.getSelected();
				if (selected > -1) {
					resources.renderTransformers(1, client.world.getObject(selected), client.editMode, blueArrowID);

					glm::vec3 transform_change = glm::vec3(0.0f);
					float multiplier = 0.1f;
					if (glfwGetKey(window, KEY_LALT) == GLFW_PRESS || glfwGetKey(window, KEY_RALT) == GLFW_PRESS) {
						multiplier = 0.02f;
					}
					if (glfwGetKey(window, KEY_UP) == GLFW_PRESS) {
						transform_change.x += multiplier;
					}
					if (glfwGetKey(window, KEY_DOWN) == GLFW_PRESS) {
						transform_change.x -= multiplier;
					}
					if (glfwGetKey(window, KEY_PAGE_UP) == GLFW_PRESS) {
						transform_change.y += multiplier;
					}
					if (glfwGetKey(window, KEY_PAGE_DOWN) == GLFW_PRESS) {
						transform_change.y -= multiplier;
					}
					if (glfwGetKey(window, KEY_LEFT) == GLFW_PRESS) {
						transform_change.z -= multiplier;
					}
					if (glfwGetKey(window, KEY_RIGHT) == GLFW_PRESS) {
						transform_change.z += multiplier;
					}

					if (client.editMode == MOVE) {
						client.world.objects[selected].pos += transform_change;
					}
					else if (client.editMode == SCALE) {
						client.world.objects[selected].sca += transform_change;
					}
					else if (client.editMode == ROTATE) {
						client.world.objects[selected].pitch -= transform_change.x * 10;
						client.world.objects[selected].yaw += transform_change.y * 10;
						client.world.objects[selected].roll += transform_change.z * 10;
					}

				}


			}
		}




		frames++;
		if (glfwGetTime() - timer > 1.0f) {
			//Every 1 second.
			timer++;
			std::cout << "FPS:" << frames << std::endl;
			frames = 0;
		}

		timeSinceScene++;
		if (timeSinceScene == 30) {
			//audio.update(client.world);
			timeSinceScene = 0;

		}

		engine_interface.update(InGame, client, physics, resources, scripts);
		glViewport(engine_interface.xDisplacement, height - ( 9 * (width - engine_interface.xDisplacement) / 16), width - engine_interface.xDisplacement, 9 * (width - engine_interface.xDisplacement) / 16);
	
		if (tempInGame != InGame) {
			if (InGame == false) {
				client.world.loadBinary(client.WorkingDir + "\\Scenes\\" + client.world.name);
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				client.viewingCursor = true;
			}
			else {
				client.world.saveBinary(client.WorkingDir);
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				client.viewingCursor = false;
				physics.OnStart(client.world, window);

			}
		}



		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}

	resources.end();
	audio.clean();

	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
