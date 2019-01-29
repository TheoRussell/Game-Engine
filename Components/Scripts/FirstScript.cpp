#include "FirstScript.h"



FirstScript::FirstScript()
{
	SetName("FirstScript");
}


FirstScript::~FirstScript()
{
}


void FirstScript::OnStart() {
	AddForce(glm::vec3(0.0f, 3000.0f, 0.0));
}


void FirstScript::OnFixedUpdate(float deltaTime) {
	AddTorque(glm::vec3(0.0f, 0.0f, 100.0f));
	AddForce(glm::vec3(0.0f, -50.0f, 0.0));

	if (KeyDown(KEY_LEFT)) {
		AddForce({ 0.0f, 0.0f, -500.0f });
	}
	if (KeyDown(KEY_RIGHT)) {
		AddForce({ 0.0f, 0.0f, 500.0f });
	}
	if (KeyDown(KEY_UP)) {
		AddForce({ 0.0f, 100.0f, 0.0f });
	}
}


void FirstScript::OnCollide(Collision collision)
{

}
