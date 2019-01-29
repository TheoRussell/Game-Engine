#include "DebugMsg.h"

DebugMsg::DebugMsg() {
	SetName("DebugMsg"); //DO NOT EDIT 
}

DebugMsg::~DebugMsg() {

}

void DebugMsg::OnStart() {
	SetCursorVisible(true);
}

void DebugMsg::OnUpdate() {
	if (KeyDown(GLFW_KEY_M)) {
		VariableAdd("msg", (unsigned int)1);
		DebugPrint("Helloa!(" + std::to_string(VariableGetUInt("msg")) + ")");
	}

	if (KeyPressed(GLFW_KEY_C)) {
		SetCursorVisible(!GetCursorVisible());
	}

	VariableAdd("timer", (int)1);
	VariableSet("bump", (float)sin(VariableGetInt("timer") / 30.0f));

	ComponentManager m = getComponents();
	m.pointLights.at(0).light.setDiffuse(glm::vec3(0.0f, 0.0f, VariableGetFloat("bump")));
	setComponents(m);


	DebugPrint(std::to_string(VariableGetFloat("bump")));
	
}

