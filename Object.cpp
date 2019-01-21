#include "Object.h"

Object::~Object() {

}

Object Object::operator-(const Object & other)
{
	Object o(pos - other.pos, sca - other.sca, pitch - other.pitch, yaw - other.yaw, roll - other.roll);
	o.components = components;
	return o;
}


Object Object::operator*(const double & deltaTime)
{
	Object o(pos * (float)deltaTime, sca * (float)deltaTime, pitch * deltaTime, yaw * deltaTime, roll * deltaTime);
	o.components = components;
	return o;
}

Object::Object() {
	pos = glm::vec3(0.0f, 0.0f, 0.0f);
	sca = glm::vec3(1.0f, 1.0f, 1.0f);
	pitch = 0;
	roll = 0;
	yaw = 0;
	au_source.push_back(AudioSource());
}

Object::Object(glm::vec3 p) {
	pos = p;
	sca = glm::vec3(1.0f, 1.0f, 1.0f);
	pitch = 0;
	roll = 0;
	yaw = 0;
	au_source.push_back(AudioSource());
}

Object::Object(unsigned int id) {
	pos = glm::vec3(0.0f, 0.0f, 0.0f);
	sca = glm::vec3(1.0f, 1.0f, 1.0f);
	pitch = 0;
	roll = 0;
	yaw = 0;
	au_source.push_back(AudioSource());
	//components.addModel(id);
	std::cout << "T" << std::endl;
}

Object::Object(glm::vec3 _pos, glm::vec3 scale, float _pitch, float _yaw, float _roll) {
	pos = _pos;
	sca = scale;
	pitch = _pitch;
	roll = _roll;
	yaw = _yaw;
	au_source.push_back(AudioSource());
}


//Used to get object data from a script.
void Object::GetScriptData(ComponentScript* script, unsigned int id) {
	//Object data.
	components = script->getComponents();
	pos = script->GetPosition();
	sca = script->GetScale();
	pitch = script->GetPitch();
	roll = script->GetRoll();
	yaw = script->GetYaw();
	exitGame = script->getExit();
	physicsBody.mass = script->GetMass();

	raycasts = script->getRaycasts();
	script->clearRaycasts();

	newScene = script->getNewScene();

	//Forces
	forces.insert(forces.end(), script->GetAllForces()->begin(), script->GetAllForces()->end());
	script->clearForces();
	physicsBody.linear_velocity = script->GetLinearVelocity();

	//Torques
	torques.insert(torques.end(), script->GetAllTorques()->begin(), script->GetAllTorques()->end());
	script->clearTorques();
	physicsBody.angular_velocity = script->GetAngularVelocity();

	//Variables.
	componentScriptData[id] = script->getScriptData();

	//New actors.
	std::vector<Actor> actors = script->getNewActors();
	if (actors.size() > 0) {
		newActors.insert(newActors.end(), actors.begin(), actors.end());
	}
	script->clearActors();

	//Delete object.
	deleteSelf = script->isDeleted();
}

void Object::PrepareScript(ComponentScript* script, unsigned int id, GLFWwindow *_window) {
	script->update(pos, sca, pitch, roll, yaw, components); //Run script.
	script->SetMass(physicsBody.mass);
	script->SetLinearVelocity(physicsBody.linear_velocity);
	script->setDelete(false);
	script->setScriptData(componentScriptData[id], _window);
}

//Initialises the script with all it's variables.
void Object::startScript(ComponentScript* script, unsigned int scriptID, GLFWwindow *_window) {
	script->update(pos, sca, pitch, roll, yaw, components); //Run script.
	script->SetMass(physicsBody.mass);
	script->SetLinearVelocity(physicsBody.linear_velocity);
	script->setDelete(false);
	script->OnStart();
	//Update obj data.
	GetScriptData(script, scriptID);
	componentScriptData[scriptID].enabled = true;
}

void Object::OnStart(ComponentScript* script, unsigned int scriptID, GLFWwindow *_window) {
	script->update(pos, sca, pitch, roll, yaw, components); //Run script.
	script->SetMass(physicsBody.mass);
	script->SetLinearVelocity(physicsBody.linear_velocity);
	script->setDelete(false);
	componentScriptData[scriptID].variable_bool.clear();
	componentScriptData[scriptID].variable_int.clear();
	componentScriptData[scriptID].variable_float.clear();
	componentScriptData[scriptID].variable_string.clear();
	componentScriptData[scriptID].variable_uint.clear();
	script->setScriptData(componentScriptData[scriptID], _window);
	script->OnStart();
	
	GetScriptData(script, scriptID);
}

void Object::OnUpdate(ComponentScript* script, unsigned int scriptID, GLFWwindow *_window) {
	PrepareScript(script, scriptID, _window);
	script->OnUpdate();

	GetScriptData(script, scriptID);

	/*components = script->getComponents();
	pos = script->GetPosition();
	sca = script->GetScale();
	pitch = script->GetPitch();
	roll = script->GetRoll();
	yaw = script->GetYaw();
	raycasts = script->getRaycasts();

	newScene = script->getNewScene();
	exitGame = script->getExit();
	script->clearRaycasts();
	componentScriptData[scriptID] = script->getScriptData();
	std::vector<Actor> actors = script->getNewActors();
	if (actors.size() > 0) {
		newActors.insert(newActors.end(), actors.begin(), actors.end());
	}
	script->clearActors();
	deleteSelf = script->isDeleted();*/
}

void Object::OnFixedUpdate(ComponentScript* script, unsigned int scriptID, GLFWwindow *_window, float deltaTime) {
	//Updating script information.
	PrepareScript(script, scriptID, _window);
	//Running script.
	script->OnFixedUpdate(deltaTime);
	GetScriptData(script, scriptID);
}

void Object::OnCollide(Collision c, ComponentScript* script, unsigned int scriptID, GLFWwindow *_window) {
	PrepareScript(script, scriptID, _window);
	script->OnCollide(c); //Run script
						   //Update obj data.
	GetScriptData(script, scriptID);
}

void Object::OnRaycast(RayHit rc, ComponentScript* script, unsigned int scriptID, GLFWwindow *_window) {
	PrepareScript(script, scriptID, _window);
	script->OnRaycast(rc); //Run script
						   //Update obj data.
	GetScriptData(script, scriptID);
}


unsigned int Object::addCamera() {
	Camera camera;
	return components.addCamera(camera);
}

unsigned int Object::addCamera(Camera camera) {
	return components.addCamera(camera);
}

unsigned int Object::addPointLight() {
	PointLight light;
	return components.addPointLight(light);
}

unsigned int Object::addPointLight(PointLight light) {
	return components.addPointLight(light);
}

unsigned int Object::addModel() {
	return components.addModel(0);
}

unsigned int Object::addModel(unsigned int ID) {
	return components.addModel(ID);
}

void Object::select() {
	selected = true;
}

void Object::deselect() {
	selected = false;
}

bool Object::isSelected() {
	return selected;
}

Transform Object::getTransformStruct() {
	Transform t;
	//t.pitch = pitch;
	//t.roll = roll;
	//t.yaw = yaw;
	t.pitch = 0.0f;
	t.roll = 0.0f;
	t.yaw = 0.0f;
	t.scale = sca;
	t.position = pos;
	return t;
}

void Object::addScript(unsigned int ID) {
	componentScriptIDs.push_back(ID);
	DataStorage ds;
	componentScriptData.insert(std::pair<unsigned int, DataStorage>(ID, ds));
}

void Object::deleteScript(unsigned int ID) {
	unsigned int index = 0;
	for each (unsigned int cs in componentScriptIDs) {
		if (cs == ID) {
			componentScriptData.erase(ID);
			componentScriptIDs.erase(componentScriptIDs.begin() + index);
			break;
		}
		index++;
	}
}

float Object::distanceTo(glm::vec3 position) {
	float xChange = position.x - pos.x;
	float yChange = position.y - pos.y;
	float zChange = position.z - pos.z;
	
	return sqrt(xChange*xChange + yChange*yChange + zChange*zChange);
}

glm::mat4 Object::getTransform() {
	glm::mat4 result = glm::mat4(1.0f);

	result = glm::translate(result, pos);
	result = glm::rotate(result, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f)); //X
	result = glm::rotate(result, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f)); //Y
	result = glm::rotate(result, glm::radians(roll), glm::vec3(0.0f, 0.0f, 1.0f)); //Z
	glm::translate(result, glm::vec3(0.0f, 0.0f, 0.0f));
	result = glm::scale(result, sca);
	return result;
}

glm::vec3 Object::getFront() {
	glm::vec3 result;
	result.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	result.y = sin(glm::radians(pitch));
	result.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	return glm::normalize(result);
}

glm::vec3 Object::getPos() {
	return pos;
}

void Object::rotate(std::string axis, float degree) {
	if (axis == "x") {
		roll += degree;
	}
	else if (axis == "y") {
		pitch += degree;
	}
	else if (axis == "z") {
		yaw += degree;
	}
}

void Object::setRot(std::string axis, float angle) {
	if (axis == "x") {
		roll = angle;
	}
	else if (axis == "y") {
		pitch = angle;
	}
	else if (axis == "z") {
		yaw = angle;
	}
}

void Object::move(glm::vec3 amount) {
	pos += amount;
}

void Object::scale(glm::vec3 scalars) {
	sca += scalars;
}

void Object::setScale(glm::vec3 scale) {
	sca = scale;
}

void Object::setPos(glm::vec3 _pos) {
	pos = _pos;
}

void Object::moveTo(glm::vec3 p) {
	pos = p;
}