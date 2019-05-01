

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
#include "Components\Scripts\Bloxorz.h"
#include "Components\Scripts\Movement.h"
#include "Components\Scripts\walking.h"
#include "Components\Scripts\Bullet.h"
#include "Components\Scripts\Reaction.h"
//<\@GENERATED CODE@SCRIPT IMPORT@>//


void framebuffer_size_callback(GLFWwindow* window, int width, int height);


float deltaTime = 0.0f;
float lastFrame = 0.0f;

int width = 1920;
int height = 1080;


CONST double render_update = 1.0 / 60.0;
CONST double physics_update = render_update * 2.0;


void GLFWErrorHandling(int, const char* code) {
	//Errors will be called to here if they aren't handled.
}


int main(void)
{	
	std::cout << "OpenGL Development: Theodore Russell" << std::endl;
	std::cout << "Game Engine - V1.0" << std::endl;

	glfwSetErrorCallback(GLFWErrorHandling);

	/* Initialize the library */
	if (!glfwInit())
		return -1; //Exits the program if it can't initialise.

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
	
	//Getting a list of all monitors.
	int count;
	GLFWmonitor** monitors = glfwGetMonitors(&count);
	//Identifying the primary monitor. - In future iterations, the user could choose the starting monitor.
	GLFWmonitor* monitor = monitors[0];
	const GLFWvidmode* video = glfwGetVideoMode(monitor);
	width = video->width;
	height = video->height;
	


	//glfwSetWindowMonitor(window, monitor, 0, 0, width, height, video->refreshRate);
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

	//Instantiates the core engine sections.
	ResourceManager resources;
	PhysicsEngine physics;
	AudioEngine audio;
	ClientHandler client;
	ScriptManager scripts;
	ComponentManager components;
	EngineInterface engine_interface;

	engine_interface.width = width;
	engine_interface.height = height;


	//Plays the starting sound of the engine running.
	//Possibly disable in future iterations.
	int LoadSound = AudioSource::generate();
	AudioSource::playAudio(LoadSound, AudioStream::GetBuffer("car"));


	if (client.closeProgram) {
		//End program.
		resources.end();
		glfwTerminate();
		exit(1);
	}

	//Centers cursor to middle of the application.
	glfwSetCursorPos(window, width / 2, height / 2);


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
		//Displays the projects to the user.
		engine_interface.ProjectsTab(client, resources);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}



	unsigned int blueArrowID = 0;
	//Displaying the collision boxes requires a cube model.
	Model cube = Model("Cube", "src\\models\\block.obj");

	if (client.engineView == PRODUCT) {
		//Activates the final game.
		client.WorkingDir = client.project.getActive();
		client.loadProject();
		resources.loadSavedActors(client.WorkingDir);
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
		//Important to have a reference to the starting arrow type, when displaying transformations / selected obj.
		blueArrowID = resources.addModel(&arrow_blue);
		resources.addModel(&arrow_green);
		resources.addModel(&arrow_red);
		resources.addModel(&arrow_scale_blue);
		resources.addModel(&arrow_scale_green);
		resources.addModel(&arrow_scale_red);
		resources.addModel(&arrow_rotate_blue);
		resources.addModel(&arrow_rotate_green);
		resources.addModel(&arrow_rotate_red);
		//The sphere is for rendering objects without a body component.
		Model sphere = Model("Sphere", "src\\models\\sphere.obj");
		resources.addModel(&sphere);

	}


	
	//<@GENERATED CODE@SCRIPTS@>//
	Bloxorz script_Bloxorz;
	physics.addScript(&script_Bloxorz);
	Movement script_Movement;
	physics.addScript(&script_Movement);
	walking script_walking;
	physics.addScript(&script_walking);
	Bullet script_Bullet;
	physics.addScript(&script_Bullet);
	Reaction script_Reaction;
	physics.addScript(&script_Reaction);
	//<\@GENERATED CODE@SCRIPTS@>//

	int frames = 0;
	bool InGame = false;

	client.setScroll(window); //Allows mouse scroll wheel input.






	/* Loop until the user closes the window */
	physics.start(client.world, window);
	float timer = (float)glfwGetTime();
	bool tempInGame = InGame;

	double lastFrame = glfwGetTime();
	double updateLag = 0.0;

	//Loads the starting interface.
	engine_interface.loadInterface(client.WorkingDir + "Interfaces\\" + client.project.startInterface + ".gui", client, physics);


	while (!glfwWindowShouldClose(window))
	{

		tempInGame = InGame;


		//Calculates the time since the last render / iteration.
		double currentFrame = glfwGetTime();
		double frameDelay = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//This is to calc how many times the physics must be updated to keep up before the next render.
		updateLag += frameDelay;


		Scene lastScene = client.world;

		if (client.engineView == UIEditor) {
			resources.clearDisplay();
			client.updateScroll();
		} //We don't render the viewport or run the game while editing the GUI.
		else {
			while (updateLag >= physics_update) {
				client.update(window);
				InGame = client.InGame;
				if (InGame) {
					//Updating the font sizes if the screen changes dimensions.
					if (engine_interface.width != width | engine_interface.height != height) {
						engine_interface.FrameSizeChange(width, height);
						engine_interface.width = width;
						engine_interface.height = height;
					}
					//Updating each of the core sections.
					engine_interface.OnUpdate(physics_update, client, physics, resources);
					physics.update(client.world, physics_update, client.project.directory);
					audio.update(client.world);
					engine_interface.mouseClicked = false;

					//Changing interface if a script has requested.
					if (physics.newUI != "") {
						engine_interface.loadInterface(client.WorkingDir + "interfaces\\" + physics.newUI + ".gui", client, physics);
						physics.newUI = "";
					}

					//Saves processing time. Spawning new actors from scripts.
					if (physics.newActorRequest) {
						int index = 0;
						for (Object o : client.world.objects) {
							bool addedActors = false;
							for (Actor act : o.newActors) {
								//Places the actor in the world as a new object.
								unsigned int newObj = client.world.addObject(resources.getActor(act.name, &engine_interface.debug_messages), true); //Passes in the debug message dictionary for errors to be displayed in game if the actor isn't found.
								client.world.objects[newObj].pos += act.pos;
								client.world.objects[newObj].sca *= act.scale;
								client.world.objects[newObj].roll += act.roll;
								client.world.objects[newObj].pitch += act.pitch;
								client.world.objects[newObj].yaw += act.yaw;
								addedActors = true;
							}
							//Clears the new actors vector so they aren't spawned in the next game tick.
							o.newActors.clear();
							client.world.objects[index] = o;
							index++;
							if (addedActors) {
								break;
							}
						}
					}


				}

				if (physics.exitGame) {
					client.closeProgram = true;
				}

				updateLag -= physics_update;
			}


			//Rendering the scene.
			if (!client.closeProgram) {

				//Calculating time since last frame.
				double deltaTime = updateLag / physics_update;
				//Interpolating the scene to create smooth motions.
				Scene interpolatedScene = client.world.interpolate(lastScene, deltaTime);
				resources.setMatrices(client.world.getCamera(InGame).camera.viewMatrix(), projection);
				//Updates the viewport to have the correct position (ie, in top right corner, or covering the whole application).
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
		}




		//Calculating FPS.
		frames++;
		if (glfwGetTime() - timer > 1.0f) {
			//Every 1 second.
			timer++;
			std::cout << "FPS:" << frames << std::endl;
			frames = 0;
		}

		//Displaying UI.
		engine_interface.update(InGame, client, physics, resources, scripts);
	
		if (tempInGame != InGame) {
			//When changing playing/exiting the game, it reloads the scene.
			//It is necessary as playing modifies save data however, it should revert back to start when no longer testing.
			if (InGame == false) {
				client.world.loadBinary(client.WorkingDir + "\\Scenes\\" + client.world.name);
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				client.viewingCursor = true;
			}
			else {
				//When entering game, saves current scene.
				client.world.saveBinary(client.WorkingDir);
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				client.viewingCursor = false;
				physics.OnStart(client.world, window);

			}
		}


		if (lastScene.name != client.world.name) {
			physics.OnStart(client.world, window);
		}


		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}

	//Cleaning up memory.
	resources.end();
	audio.clean();

	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int _width, int _height)
{	
	//This function is called when the window changes dimensions.
	glViewport(0, 0, _width, _height);
	width = _width;
	height = _height;
}
