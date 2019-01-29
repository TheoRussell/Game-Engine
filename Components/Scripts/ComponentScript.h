#pragma once

#define SCRIPT_H

#include <map>
#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include "GLM/gtc/type_ptr.hpp"
#include "Components\ComponentManager.h"
#include "GLFW\glfw3.h"
#include "imgui\imgui.h"


#ifndef UIELEMENT_H
#include "UIElements.h"
#endif

#include "AudioEngine\AudioSource.h"


//Creating more engine driven key constants.
#define MOUSE_LEFT 0
#define MOUSE_RIGHT 1
#define MOUSE_MIDDLE 2
#define KEY_0 GLFW_KEY_0
#define KEY_1 GLFW_KEY_1
#define KEY_2 GLFW_KEY_2
#define KEY_3 GLFW_KEY_3
#define KEY_4 GLFW_KEY_4
#define KEY_5 GLFW_KEY_5
#define KEY_6 GLFW_KEY_6
#define KEY_7 GLFW_KEY_7
#define KEY_8 GLFW_KEY_8
#define KEY_9 GLFW_KEY_9
#define KEY_A GLFW_KEY_A
#define KEY_B GLFW_KEY_B
#define KEY_C GLFW_KEY_C
#define KEY_D GLFW_KEY_D
#define KEY_E GLFW_KEY_E
#define KEY_F GLFW_KEY_F
#define KEY_G GLFW_KEY_G
#define KEY_H GLFW_KEY_H
#define KEY_H GLFW_KEY_H
#define KEY_I GLFW_KEY_I
#define KEY_J GLFW_KEY_J
#define KEY_K GLFW_KEY_K
#define KEY_L GLFW_KEY_L
#define KEY_M GLFW_KEY_M
#define KEY_N GLFW_KEY_N
#define KEY_O GLFW_KEY_O
#define KEY_P GLFW_KEY_P
#define KEY_Q GLFW_KEY_Q
#define KEY_R GLFW_KEY_R
#define KEY_S GLFW_KEY_S
#define KEY_T GLFW_KEY_T
#define KEY_U GLFW_KEY_U
#define KEY_V GLFW_KEY_V
#define KEY_W GLFW_KEY_W
#define KEY_X GLFW_KEY_X
#define KEY_Y GLFW_KEY_Y
#define KEY_Z GLFW_KEY_Z
#define KEY_GRAVE_ACCENT GLFW_KEY_GRAVE_ACCENT
#define KEY_LBRACKET GLFW_KEY_LEFT_BRACKET
#define KEY_RBRACKET GLFW_KEY_RIGHT_BRACKET
#define KEY_LCONTROL GLFW_KEY_LEFT_CONTROL
#define KEY_RCONTROL GLFW_KEY_RIGHT_CONTROL
#define KEY_LALT GLFW_KEY_LEFT_ALT
#define KEY_RALT GLFW_KEY_RIGHT_ALT
#define KEY_LSHIFT GLFW_KEY_LEFT_SHIFT
#define KEY_RSHIFT GLFW_KEY_RIGHT_SHIFT
#define KEY_SPACE GLFW_KEY_SPACE
#define KEY_BACKSPACE GLFW_KEY_BACKSPACE
#define KEY_APOSTROPHE GLFW_KEY_APOSTROPHE
#define KEY_ESCAPE GLFW_KEY_ESCAPE
#define KEY_ENTER GLFW_KEY_ENTER
#define KEY_TAB GLFW_KEY_TAB
#define KEY_EQUAL GLFW_KEY_EQUAL
#define KEY_MINUS GLFW_KEY_MINUS
#define KEY_PERIOD GLFW_KEY_PERIOD
#define KEY_SEMICOLON GLFW_KEY_SEMICOLON
#define KEY_COMMA GLFW_KEY_COMMA
#define KEY_SLASH GLFW_KEY_SLASH
#define KEY_BACKSLASH GLFW_KEY_BACKSLASH
#define KEY_INSERT GLFW_KEY_INSERT
#define KEY_DELETE GLFW_KEY_DELETE

#define KEY_LEFT GLFW_KEY_LEFT
#define KEY_RIGHT GLFW_KEY_RIGHT
#define KEY_UP GLFW_KEY_UP
#define KEY_DOWN GLFW_KEY_DOWN

#define KEY_PRTSCRN GLFW_KEY_PRINT_SCREEN
#define KEY_NUM_LOCK GLFW_KEY_NUM_LOCK
#define KEY_SCROLL_LOCK GLFW_KEY_SCROLL_LOCK
#define KEY_CAPS_LOCK GLFW_KEY_CAPS_LOCK
#define KEY_END GLFW_KEY_END
#define KEY_HOME GLFW_KEY_HOME
#define KEY_PAGE_UP GLFW_KEY_PAGE_UP
#define KEY_PAGE_DOWN GLFW_KEY_PAGE_DOWN

#define KEY_F1 GLFW_KEY_F1
#define KEY_F2 GLFW_KEY_F2
#define KEY_F3 GLFW_KEY_F3
#define KEY_F4 GLFW_KEY_F4
#define KEY_F5 GLFW_KEY_F5
#define KEY_F6 GLFW_KEY_F6
#define KEY_F7 GLFW_KEY_F7
#define KEY_F8 GLFW_KEY_F8
#define KEY_F9 GLFW_KEY_F9
#define KEY_F10 GLFW_KEY_F10
#define KEY_F11 GLFW_KEY_F11
#define KEY_F12 GLFW_KEY_F12
#define KEY_F13 GLFW_KEY_F13
#define KEY_F14 GLFW_KEY_F14
#define KEY_F15 GLFW_KEY_F15
#define KEY_F16 GLFW_KEY_F16
#define KEY_F17 GLFW_KEY_F17
#define KEY_F18 GLFW_KEY_F18
#define KEY_F19 GLFW_KEY_F19
#define KEY_F20 GLFW_KEY_F20
#define KEY_F21 GLFW_KEY_F21
#define KEY_F22 GLFW_KEY_F22
#define KEY_F23 GLFW_KEY_F23
#define KEY_F24 GLFW_KEY_F24
#define KEY_F25 GLFW_KEY_F25

#define KEY_PAD_0 GLFW_KEY_KP_0
#define KEY_PAD_1 GLFW_KEY_KP_1
#define KEY_PAD_2 GLFW_KEY_KP_2
#define KEY_PAD_3 GLFW_KEY_KP_3
#define KEY_PAD_4 GLFW_KEY_KP_4
#define KEY_PAD_5 GLFW_KEY_KP_5
#define KEY_PAD_6 GLFW_KEY_KP_6
#define KEY_PAD_7 GLFW_KEY_KP_7
#define KEY_PAD_8 GLFW_KEY_KP_8
#define KEY_PAD_9 GLFW_KEY_KP_9
#define KEY_PAD_DIVIDE GLFW_KEY_KP_DIVIDE
#define KEY_PAD_MULTIPLY GLFW_KEY_KP_MULTIPLY
#define KEY_PAD_SUBTRACT GLFW_KEY_KP_SUBTRACT
#define KEY_PAD_ADD GLFW_KEY_KP_ADD
#define KEY_PAD_ENTER GLFW_KEY_KP_ENTER
#define KEY_PAD_EQUAL GLFW_KEY_KP_EQUAL



struct ImpulseTransfer {
	glm::vec3 force = glm::vec3(0.0f, 0.0f, 0.0f); //net force (N).
	float time = 1.0f; //how long it's applied (seconds).
};


struct Collision {
	std::string hit_label = "";
	float hit_mass = 50.0f;
	glm::vec3 hit_linear_velocity = glm::vec3(0.0f);
	glm::vec3 hit_angular_velocity = glm::vec3(0.0f);

	bool collided = false;
	unsigned int objID = 0;
	glm::vec3 impact_position = glm::vec3(0.0f,0.0f,0.0f);
	float distanceX = 0.0f;
	float distanceY = 0.0f;
	float distanceZ = 0.0f;
	glm::vec3 b2_max;
	glm::vec3 b2_min;
	glm::vec3 b1_max;
	glm::vec3 b1_min;
};

struct RayHit {
	float length = 6.0f;
	float increments = 0.1f;
	glm::vec3 origin = glm::vec3(0.0f,0.0f,0.0f);
	glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);
	Collision collision;
};

struct DataStorage {
	std::map<std::string, int> variable_int;
	std::map<std::string, unsigned int> variable_uint;
	std::map<std::string, bool> variable_bool;
	std::map<std::string, float> variable_float;
	std::map<std::string, std::string> variable_string;
	bool enabled = true;
};


struct Actor {
	std::string name;
	glm::vec3 pos;
	glm::vec3 scale;
	float pitch;
	float yaw;
	float roll;
};



class Scene;

class ComponentScript
{
public:

	ComponentScript();
	~ComponentScript();
	ComponentScript(glm::vec3 _pos, glm::vec3 _sca, float _pitch, float _roll, float _yaw);
	ComponentScript(glm::vec3 _pos, glm::vec3 _sca, float _pitch, float _roll, float _yaw, ComponentManager _components);
	void update(glm::vec3 _pos, glm::vec3 _sca, float _pitch, float _roll, float _yaw);
	void NewAudioSource(std::string name);
	void SetSound(std::string source, std::string sound);
	void PlaySound(std::string source);
	void StopSound(std::string source);
	void ChangeScene(std::string name);
	std::string getNewScene();
	void resetNewScene();
	void ChangeUI(std::string name);
	std::string getNewUI();
	void resetNewUI();
	void ExitGame();
	bool getExit();
	void SetCursorVisible(bool val);
	bool GetCursorVisible();
	void update(glm::vec3 _pos, glm::vec3 _sca, float _pitch, float _roll, float _yaw, ComponentManager _components);

	std::vector<std::string> getDebugText();


	void DebugPrint(std::string text = "");

	DataStorage getScriptData();
	void setScriptData(DataStorage ds, GLFWwindow * _window);

	void setScriptWindow(GLFWwindow * _window);


	bool AddInterfaceReference(std::string s);
	std::map<std::string, InterfaceItem*>* GetInterfacePointers();
	std::vector<std::string>* getNewInterfaceRefs();


	std::vector<Actor> getNewActors();
	void clearActors();
	void SpawnActor(std::string name);
	void SpawnActor(std::string name, glm::vec3 position, glm::vec3 scale, float roll, float pitch, float yaw);

	glm::vec3 GetLinearVelocity();

	void SetLinearVelocity(glm::vec3 velocity);

	glm::vec3 GetAngularVelocity();

	void SetAngularVelocity(glm::vec3 velocity);


	//Forces
	std::vector<glm::vec3>* GetAllForces();
	void clearForces();
	void AddForce(glm::vec3 Force);

	std::vector<glm::vec3>* GetAllTorques();
	void clearTorques();
	void AddTorque(glm::vec3 Torque);





	std::vector<RayHit> getRaycasts();
	void clearRaycasts();
	void Raycast();
	void Raycast(glm::vec3 origin, glm::vec3 direction);
	void Raycast(glm::vec3 origin, glm::vec3 direction, float length);
	void Raycast(glm::vec3 origin, glm::vec3 direction, float length, float increments);
	void updateKeys();
	bool KeyPressed(int key_id);
	bool KeyDown(int key_id);
	bool MouseDown(int mouse_id);
	std::string GetName();
	glm::vec3 GetPosition();
	glm::vec3 GetScale();
	float GetPitch();
	float GetYaw();
	float GetRoll();
	void SetPitch(float degree);
	void SetRoll(float degree);
	void SetYaw(float degree);
	void SetName(std::string _name);
	void SetPosition(glm::vec3 _pos);
	void SetScale(glm::vec3 _sca);
	void Move(glm::vec3 amount);
	void Scale(glm::vec3 scalars);
	virtual void OnCollide(Collision collision);
	virtual void OnStart();
	virtual void OnRaycast(RayHit RayData);
	virtual void OnUpdate();
	virtual void OnFixedUpdate(float deltaTime);
	//Variables
	ComponentManager getComponents();
	void setComponents(ComponentManager _components);

	void NewVariable(std::string name, int value);
	void NewVariable(std::string name, unsigned int value);
	void NewVariable(std::string name, float value);
	void NewVariable(std::string name, bool value);
	void NewVariable(std::string name, std::string value);
	void VariableSet(std::string name, int value);
	void VariableSet(std::string name, unsigned int value);
	void VariableSet(std::string name, float value);
	void VariableSet(std::string name, bool value);
	void VariableSet(std::string name, std::string value);
	void VariableAdd(std::string name, int value);
	void VariableAdd(std::string name, unsigned int value);
	void VariableAdd(std::string name, float value);
	void VariableGet(std::string name, int & value);
	void VariableGet(std::string name, unsigned int &value);
	void VariableGet(std::string name, float & value);
	void VariableGet(std::string name, bool & value);
	void VariableGet(std::string name, std::string & value);
	int				VariableGetInt(std::string name);
	unsigned int	VariableGetUInt(std::string name);
	float			VariableGetFloat(std::string name);
	bool			VariableGetBool(std::string name);
	std::string		VariableGetString(std::string name);
	void deleteVariableInt(std::string name);
	void deleteVariableUInt(std::string name);
	void deleteVariableFloat(std::string name);
	void deleteVariableBool(std::string name);
	void deleteVariableString(std::string name);
	std::map<std::string, int> getAllVarInts();
	std::map<std::string, unsigned int> getAllVarUInts();
	std::map<std::string, float> getAllVarFloats();
	std::map<std::string, bool> getAllVarBools();
	std::map<std::string, std::string> getAllVarStrings();
	void setAllVarInts(std::map<std::string, int> values);
	void setAllVarUInts(std::map<std::string, unsigned int> values);
	void setAllVarFloats(std::map<std::string, float> values);
	void setAllVarBools(std::map<std::string, bool> values);
	void setAllVarStrings(std::map<std::string, std::string> values);

	glm::vec3 GetForward();
	glm::vec3 GetRight();


	bool isDeleted();
	void setDelete(bool toDelete);
	void Destroy();


	void SetMass(float mass);
	float GetMass();




	std::map<std::string, InterfaceItem*> UI_elements;
	std::vector<std::string> UI_references;



	bool keys[351] = {};
	std::string newScene = "";
	std::string newUI = "";
private:
	ComponentManager components;
	std::string name;
	glm::vec3 pos;
	glm::vec3 sca;
	float pitch;
	float yaw;
	float roll;
	float deltaTime;
	float enabled;
	GLFWwindow *window;
	Scene *scene = nullptr;
	bool deleteObj = false;
	bool exit = false;

	float mass = 50.0f; //Physics mass


	//Forces
	glm::vec3 linear_velocity;
	glm::vec3 angular_velocity;
	std::vector<glm::vec3> torques;
	std::vector<glm::vec3> forces;



	std::vector<RayHit> raycasts;
	std::vector<Actor> newActors;


	std::vector<ImpulseTransfer> newImpulses;


	std::map<std::string, int> variable_int;
	std::map<std::string, unsigned int> variable_uint;
	std::map<std::string, bool> variable_bool;
	std::map<std::string, float> variable_float;
	std::map<std::string, std::string> variable_string;


	std::vector<std::string> debug_text;
};

