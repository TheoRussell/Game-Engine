#include "MoveLightScript.h"

MoveLightScript::MoveLightScript() {
	SetName("MoveLightScript"); //DO NOT EDIT 
}

MoveLightScript::~MoveLightScript() {

}

void MoveLightScript::OnStart() {
	NewVariable("MoveLight", true);
	NewVariable("timer2", 0.0f);
}

void MoveLightScript::OnFixedUpdate(float deltaTime) {
	float radius = 2.0f;
	SetPosition(glm::vec3(GetPosition().x, 80 * deltaTime * radius * cos(glm::radians(VariableGetFloat("timer2"))), GetPosition().z));

	//if (getComponents().pointLights.size() > 0 && getVariableBool("MoveLight")) {
	//	ComponentManager c = getComponents();
	//	c.pointLights[0].componentTransform.move(glm::vec3(sin(glm::radians(getVariableFloat("timer2"))) / 10, 0.0f, 0.0f));
	//	//c.pointLights[0].light.setDiffuse(glm::vec3(cos(glm::radians(getVariableFloat("timer2"))),1.0f, sin(glm::radians(getVariableFloat("timer2")))));
	//	setComponents(c);
	//}
	VariableSet("timer2", VariableGetFloat("timer2") + 1);
}

