
#include "ComponentScript.h"




ComponentScript::ComponentScript()
{
	name = "";
	pos = glm::vec3(0.0f,0.0f,0.0f);
	sca = glm::vec3(1.0f, 1.0f, 1.0f);
	pitch = 0;
	roll = 0;
	yaw = 0;
}

ComponentScript::ComponentScript(glm::vec3 _pos, glm::vec3 _sca, float _pitch, float _roll, float _yaw)
{
	name = "";
	pos = _pos;
	sca = _sca;
	pitch = _pitch;
	roll = _roll;
	yaw = _yaw;
}

ComponentScript::ComponentScript(glm::vec3 _pos, glm::vec3 _sca, float _pitch, float _roll, float _yaw, ComponentManager _components)
{
	pos = _pos;
	sca = _sca;
	pitch = _pitch;
	roll = _roll;
	yaw = _yaw;
	components = _components;
}

void ComponentScript::update(glm::vec3 _pos, glm::vec3 _sca, float _pitch, float _roll, float _yaw) {
	pos = _pos;
	sca = _sca;
	pitch = _pitch;
	roll = _roll;
	yaw = _yaw;
}



//Audio functions.
void ComponentScript::NewAudioSource(std::string name) {
	
}
void ComponentScript::SetSound(std::string source, std::string sound) {
	
}
void ComponentScript::PlaySound(std::string source) {

}
void ComponentScript::StopSound(std::string source) {

}




void ComponentScript::ChangeScene(std::string name) {
	newScene = name;
}
std::string ComponentScript::getNewScene() {
	return newScene;
}
void ComponentScript::resetNewScene() {
	newScene = "";
}


void ComponentScript::ChangeUI(std::string name) {
	newUI = name;
}
std::string ComponentScript::getNewUI() {
	return newUI;
}
void ComponentScript::resetNewUI() {
	newUI = "";
}



void ComponentScript::ExitGame() {
	exit = true;
}

bool ComponentScript::getExit() {
	return exit;
}

void ComponentScript::SetCursorVisible(bool val) {
	if (val) {
		int w, h;
		glfwGetWindowSize(window, &w, &h);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwSetCursorPos(window, w/2, h/2);
	}
	else {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}

bool ComponentScript::GetCursorVisible() {
	if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
		return true;
	}
	else {
		return false;
	}
}




void ComponentScript::update(glm::vec3 _pos, glm::vec3 _sca, float _pitch, float _roll, float _yaw, ComponentManager _components)
{
	pos = _pos;
	sca = _sca;
	pitch = _pitch;
	roll = _roll;
	yaw = _yaw;
	components = _components;
}

std::vector<std::string> ComponentScript::getDebugText() {
	std::vector<std::string> t = debug_text;
	debug_text.clear();
	return t;
}

void ComponentScript::DebugPrint(std::string text) {
	debug_text.push_back(text);
}

DataStorage ComponentScript::getScriptData()
{
	DataStorage ds;
	ds.variable_int = getAllVarInts();
	ds.variable_uint = getAllVarUInts();
	ds.variable_float = getAllVarFloats();
	ds.variable_bool = getAllVarBools();
	ds.variable_string = getAllVarStrings();
	ds.enabled = enabled;
	return ds;
}
void ComponentScript::setScriptData(DataStorage ds, GLFWwindow *_window) {
	setAllVarInts(ds.variable_int);
	setAllVarUInts(ds.variable_uint);
	setAllVarFloats(ds.variable_float);
	setAllVarBools(ds.variable_bool);
	setAllVarStrings(ds.variable_string);
	window = _window;
	enabled = ds.enabled;
}

void ComponentScript::setScriptWindow(GLFWwindow *_window) {
	window = _window;
}




//User Interface Functions.
bool ComponentScript::AddInterfaceReference(std::string s) {
	UI_references.push_back(s);
	return true;
}

std::map<std::string, InterfaceItem*>* ComponentScript::GetInterfacePointers() {
	return &UI_elements;
}

std::vector<std::string>* ComponentScript::getNewInterfaceRefs()
{
	return &UI_references;
}


std::vector<Actor> ComponentScript::getNewActors() {
	return newActors;
}

void ComponentScript::clearActors() {
	newActors.clear();
}

void ComponentScript::SpawnActor(std::string name) {
	SpawnActor(name, glm::vec3(0.0f), glm::vec3(1.0f), 0.0f, 0.0f, 0.0f);
}

void ComponentScript::SpawnActor(std::string name, glm::vec3 position, glm::vec3 scale, float roll, float pitch, float yaw) {
	Actor a;
	a.name = name;
	a.pitch = pitch;
	a.pos = position;
	a.scale = scale;
	a.roll = roll;
	a.yaw = yaw;
	a.pitch = pitch;
	newActors.push_back(a);
}


glm::vec3 ComponentScript::GetLinearVelocity() {
	return linear_velocity;
}

void ComponentScript::SetLinearVelocity(glm::vec3 velocity) {
	linear_velocity = velocity;
}

glm::vec3 ComponentScript::GetAngularVelocity() {
	return angular_velocity;
}

void ComponentScript::SetAngularVelocity(glm::vec3 velocity) {
	angular_velocity = velocity;
}

std::vector<glm::vec3>* ComponentScript::GetAllForces() {
	return &forces;
}

void ComponentScript::clearForces()
{
	forces.clear();
}

//Forces must be applied every frame.
void ComponentScript::AddForce(glm::vec3 Force)
{
	forces.push_back(Force);
}

std::vector<glm::vec3>* ComponentScript::GetAllTorques()
{
	return &torques;
}

void ComponentScript::clearTorques()
{
	torques.clear();
}

//Similar to forces, but for rotations.
void ComponentScript::AddTorque(glm::vec3 Torque)
{
	torques.push_back(Torque);
}



std::vector<RayHit> ComponentScript::getRaycasts() {
	return raycasts;
}

void ComponentScript::clearRaycasts() {
	raycasts.clear();
}


void ComponentScript::Raycast() {
	Raycast(pos,GetForward(), 5.0f, 0.1f);
}

void ComponentScript::Raycast(glm::vec3 origin, glm::vec3 direction) {
	Raycast(origin, direction, 5.0f, 0.1f);
}

void ComponentScript::Raycast(glm::vec3 origin, glm::vec3 direction, float length) {
	Raycast(origin, direction, length, 0.1f);
}

void ComponentScript::Raycast(glm::vec3 origin, glm::vec3 direction, float length, float increments) {
	RayHit rc;
	rc.increments = increments;
	rc.length = length;
	rc.origin = origin;
	rc.direction = direction;
	raycasts.push_back(rc);
}


void ComponentScript::updateKeys()
{
	for (int i = 0; i <= 250; i++) {
		//if (ImGui::IsKeyPressed(i)) {
		if (glfwGetKey(window,i)) {
			keys[i] = true;
		}
		else {
			keys[i] = false;
		}
	}
}

bool ComponentScript::KeyPressed(int key_id) {
	if (key_id >= 0 && key_id <= 350) {
		try {
			std::cout << keys[key_id] << "," << glfwGetKey(window, key_id) << std::endl;
			if (glfwGetKey(window, key_id) == GLFW_RELEASE) {
				if (keys[key_id] == true) {
					return true;
				}
			}
		}
		catch (std::exception ex) {
			std::cout << ex.what() << std::endl;
			return false;
		}
	}
	return false;
}

bool ComponentScript::KeyDown(int key_id) {
	if (key_id >= 0 && key_id <= 350) {
		try {
			if (glfwGetKey(window, key_id) == GLFW_PRESS) {
				keys[key_id] = true;
				return true;
			}
			else {
				keys[key_id] = false;
			}
		}
		catch (std::exception ex) {
			return false;
		}
	}

	return false;
}

bool ComponentScript::MouseDown(int mouse_id) {
	if (mouse_id >= 0) {
		try {
			if (glfwGetMouseButton(window, mouse_id) == GLFW_PRESS) {
				return true;
			}
		}
		catch (std::exception ex) {
			return false;
		}
	}
	return false;
}

//NEEDS ERROR HANDLING...
void ComponentScript::NewVariable(std::string name, int value)
{
	variable_int.insert(std::pair<std::string, int>(name, value));
}
void ComponentScript::NewVariable(std::string name, unsigned int value)
{
	variable_uint.insert(std::pair<std::string, unsigned int>(name, value));
}
void ComponentScript::NewVariable(std::string name, float value)
{
	variable_float.insert(std::pair<std::string, float>(name, value));
}
void ComponentScript::NewVariable(std::string name, bool value)
{
	variable_bool.insert(std::pair<std::string, bool>(name, value));
}
void ComponentScript::NewVariable(std::string name, std::string value)
{
	variable_string.insert(std::pair<std::string, std::string>(name, value));
}
void ComponentScript::VariableSet(std::string name, int value)
{
	variable_int[name] = value;
}
void ComponentScript::VariableSet(std::string name, unsigned int value)
{
	variable_uint[name] = value;
}
void ComponentScript::VariableSet(std::string name, float value)
{
	variable_float[name] = value;
}
void ComponentScript::VariableSet(std::string name, bool value)
{
	variable_bool[name] = value;
}
void ComponentScript::VariableSet(std::string name, std::string value)
{
	variable_string[name] = value;
}
void ComponentScript::VariableAdd(std::string name, int value)
{
	variable_int[name] += value;
}
void ComponentScript::VariableAdd(std::string name, unsigned int value)
{
	variable_uint[name] += value;
}
void ComponentScript::VariableAdd(std::string name, float value)
{
	variable_float[name] += value;
}
void ComponentScript::VariableGet(std::string name, int &value) {
	value = variable_int[name];
}
void ComponentScript::VariableGet(std::string name, unsigned int &value) {
	value = variable_uint[name];
}
void ComponentScript::VariableGet(std::string name, float &value) {
	value = variable_float[name];
}
void ComponentScript::VariableGet(std::string name, bool &value) {
	value = variable_bool[name];
}
void ComponentScript::VariableGet(std::string name, std::string &value) {
	value = variable_string[name];
}
int ComponentScript::VariableGetInt(std::string name) {
	return variable_int[name];
}
unsigned int ComponentScript::VariableGetUInt(std::string name) {
	return variable_uint[name];
}
float ComponentScript::VariableGetFloat(std::string name) {
	return variable_float[name];
}
bool ComponentScript::VariableGetBool(std::string name) {
	return variable_bool[name];
}
std::string ComponentScript::VariableGetString(std::string name) {
	return variable_string[name];
}
void ComponentScript::deleteVariableInt(std::string name)
{
	variable_int.erase(name);
}
void ComponentScript::deleteVariableUInt(std::string name)
{
	variable_uint.erase(name);
}
void ComponentScript::deleteVariableFloat(std::string name)
{
	variable_float.erase(name);
}
void ComponentScript::deleteVariableBool(std::string name)
{
	variable_bool.erase(name);
}
void ComponentScript::deleteVariableString(std::string name)
{
	variable_string.erase(name);
}

std::map<std::string, int> ComponentScript::getAllVarInts()
{
	return variable_int;
}
std::map<std::string, unsigned int> ComponentScript::getAllVarUInts()
{
	return variable_uint;
}
std::map<std::string, float> ComponentScript::getAllVarFloats()
{
	return variable_float;
}
std::map<std::string, bool> ComponentScript::getAllVarBools()
{
	return variable_bool;
}
std::map<std::string, std::string> ComponentScript::getAllVarStrings()
{
	return variable_string;
}
void ComponentScript::setAllVarInts(std::map<std::string, int> values)
{
	variable_int = values;
}
void ComponentScript::setAllVarUInts(std::map<std::string, unsigned int> values)
{
	variable_uint = values;
}
void ComponentScript::setAllVarFloats(std::map<std::string, float> values)
{
	variable_float = values;
}
void ComponentScript::setAllVarBools(std::map<std::string, bool> values)
{
	variable_bool = values;
}
void ComponentScript::setAllVarStrings(std::map<std::string, std::string> values)
{
	variable_string = values;
}

glm::vec3 ComponentScript::GetForward() {
	glm::vec3 t_front;
	t_front.x = cos(glm::radians(pitch)) * cos(glm::radians(glm::mod(yaw, 360.0f)));
	t_front.y = sin(glm::radians(pitch));
	t_front.z = cos(glm::radians(pitch)) * sin(glm::radians(glm::mod(yaw, 360.0f)));
	return glm::normalize(t_front);
}

glm::vec3 ComponentScript::GetRight() {
	return -glm::normalize(glm::cross(glm::vec3(0.0f,1.0f,0.0f), GetForward()));
}

bool ComponentScript::isDeleted()
{
	return deleteObj;
}

void ComponentScript::setDelete(bool toDelete)
{
	deleteObj = toDelete;
}

void ComponentScript::Destroy()
{
	deleteObj = true;
}

void ComponentScript::SetMass(float _mass)
{
	mass = _mass;
}

float ComponentScript::GetMass()
{
	return mass;
}

std::string ComponentScript::GetName() {
	return name;
}

glm::vec3 ComponentScript::GetPosition() {
	return pos;
}

glm::vec3 ComponentScript::GetScale() {
	return sca;
}

float ComponentScript::GetPitch() {
	return pitch;
}

float ComponentScript::GetYaw() {
	return yaw;
}

float ComponentScript::GetRoll() {
	return roll;
}

void ComponentScript::SetPitch(float degree) {
	pitch = degree;
}
void ComponentScript::SetRoll(float degree) {
	roll = degree;
}
void ComponentScript::SetYaw(float degree) {
	yaw = degree;
}

ComponentManager ComponentScript::getComponents()
{
	return components;
}

void ComponentScript::setComponents(ComponentManager _components)
{
	components = _components;
}

void ComponentScript::SetName(std::string _name) {
	name = _name;
}

void ComponentScript::Move(glm::vec3 amount) {
	pos += amount;
}

void ComponentScript::Scale(glm::vec3 scalars) {
	sca *= scalars;
}

void ComponentScript::SetPosition(glm::vec3 _pos) {
	pos = _pos;
}

void ComponentScript::SetScale(glm::vec3 _sca) {
	sca = _sca;
}

void ComponentScript::OnUpdate() {
	
}

void ComponentScript::OnFixedUpdate(float deltaTime)
{

}

void ComponentScript::OnCollide(Collision collision)
{

}

void ComponentScript::OnStart()
{

}

void ComponentScript::OnRaycast(RayHit RayData) {

}

ComponentScript::~ComponentScript()
{
}
